/**
 * @file flags-v2.hpp
 * @author shlchen (3079625093@qq.com)
 * @brief the second version for lib-flags
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef FLAGS_FLAGS_HPP
#define FLAGS_FLAGS_HPP

#include "utility"
#include "arg_type.hpp"
#include "exception"
#include "map"
#include "iomanip"

namespace ns_flags {

#define FLAGS_THROW_EXCEPTION(where, msg)                                                \
  throw std::runtime_error(std::string("[ error from 'lib-flags':'") + #where + "' ] " + \
                           msg + ". (use option '--help' to get more info)")

#define FLAGS_THROW_EXCEPTION_DEVELOPER(where, msg)                                      \
  throw std::runtime_error(std::string("[ error from 'lib-flags':'") + #where + "' ] " + \
                           msg + ". (this message is send to the developer only)")

    enum class OptionProp {
        // the option is optional
        OPTIONAL,
        // the option is required, and it's necessary
        REQUIRED
    };

    static std::ostream &operator<<(std::ostream &os, const OptionProp &obj) {
        switch (obj) {
            case OptionProp::OPTIONAL:
                os << "Optional";
                break;
            case OptionProp::REQUIRED:
                os << "Required";
                break;
        }
        return os;
    }

    struct Variable {
    public:
        std::shared_ptr<Argument> defaultValue, value;

        template<class ArgumentType>
        static auto Create(const typename ArgumentType::data_type &defaultValue) {
            return Variable(std::make_shared<ArgumentType>(defaultValue), std::make_shared<ArgumentType>(defaultValue));
        }

    protected:
        Variable(const std::shared_ptr<Argument> &defaultValue, const std::shared_ptr<Argument> &value)
                : defaultValue(defaultValue), value(value) {}


        friend std::ostream &operator<<(std::ostream &os, const Variable &variable) {
            os << "'defaultValue': " << variable.defaultValue->ValueString()
               << ", 'value': " << variable.value->ValueString();
            return os;
        }
    };

    namespace ns_priv {
        enum class OptionNameType {
            OPT_SHORT_NAME, OPT_LONG_NAME, NONE
        };

        /**
         * @brief Judge whether a parameter is an option name
         *
         * @param str the argv
         * @return true
         * @return false
         */
        static OptionNameType IsAnOption(const std::string &str) {
            if (str.size() == 2 && str.front() == '-' && Utils::IsALetter(str.at(1))) {
                return OptionNameType::OPT_SHORT_NAME;
            } else if (str.size() > 2 && str.substr(0, 2) == "--" && Utils::IsALetter(str.at(2))) {
                return OptionNameType::OPT_LONG_NAME;
            } else {
                return OptionNameType::NONE;
            }
        }

        struct Option {
        public:
            using assertor_type = std::function<std::optional<std::string>(const std::shared_ptr<Argument> &)>;

        public:
            // option's name
            const std::string optionLongName;
            const char optionShortName;

            // the value
            Variable variable;

            // to describe this option
            const std::string description;
            // the properties of this option
            const OptionProp property;
            // assertor
            assertor_type assertor;
        private:
            const char EMPTY_OPTION_SHORT_NAME = '\0';

        public:
            /**
             * @brief Construct a new Option object
             */
            Option(std::string optionLongName, char optionShortName, Variable variable,
                   std::string description, const OptionProp &property, assertor_type assertor = nullptr)
                    : optionLongName(std::move(optionLongName)), optionShortName(optionShortName),
                      variable(std::move(variable)), description(std::move(description)), property(property),
                      assertor(std::move(assertor)) {}

            ~Option() = default;

            [[nodiscard]] std::string GetOptionName() const {
                std::string optionName = "\"--" + optionLongName;
                if (optionShortName != EMPTY_OPTION_SHORT_NAME) {
                    optionName += std::string(", -") + optionShortName;
                }
                optionName += '\"';
                return optionName;
            }

            /**
             * @brief assertor for the variable's value
             */
            void AssertOptionValue() const {
                if (this->assertor == nullptr) { return; }

                if (auto msg = this->assertor(this->variable.value);msg) {
                    FLAGS_THROW_EXCEPTION(
                            AssertOptionValue,
                            "the value(s) for option " + GetOptionName() + " is(are) invalid: \"" + *msg + "\""
                    );
                }
            }

            /**
             * @brief override operator '<<' for type 'Option'
             */
            friend std::ostream &operator<<(std::ostream &os, const Option &obj) {
                os << std::boolalpha;
                os << '{';
                os << "'optionLongName': \"" << obj.optionLongName << "\", ";
                os << "'optionShortName': \"" << obj.optionShortName << "\", ";
                os << obj.variable << ", ";
                os << "'desc': \"" << obj.description << "\", ";
                os << "'prop': " << obj.property << ", ";
                os << "'argType': " << obj.variable.defaultValue->TypeNameString() << ", ";
                os << "'hasAssertor': " << (obj.assertor != nullptr);
                os << '}';
                return os;
            };
        };
    }

    class OptionParser {
    public:
        template<class ArgumentType>
        using assertor_type = std::function<std::optional<std::string>(const typename ArgumentType::data_type &)>;

    private:
        // Determine whether to automatically generate corresponding content
        // If the user explicitly sets the corresponding content, we refuse to generate it automatically
        bool _autoGenHelpDocs;
        bool _autoGenVersion;
        const std::string DEFAULT_OPTION_LONG_NAME = "def-opt";
        const char EMPTY_OPTION_SHORT_NAME = '\0';
        std::string _progDescription;

        // data
        std::unordered_map<std::string, ns_priv::Option> _optLongNameToOpt;
        std::unordered_map<char, std::string> _optShortNameToOptLongName;

    public:
        OptionParser() : _autoGenHelpDocs(true), _autoGenVersion(true) {
            // add help and version options
            // Note that the address corresponding to the variable is nullptr
            AddOption<Version>("version", 'v', "", "show the version message and exit", OptionProp::OPTIONAL, nullptr);
            AddOption<Version>("help", 'h', "", "show this help message and exit", OptionProp::OPTIONAL, nullptr);
        }

        const std::unordered_map<std::string, ns_priv::Option> &GetOptionMap() const {
            return _optLongNameToOpt;
        }

        /**
         * @brief Set the Version
         */
        void SetVersion(const std::string &version) {
            GetVersionOption().variable.defaultValue->DataFromStringVector({version});
            this->_autoGenVersion = false;
        }

        /**
         * @brief Set the Help Docs
         */
        void SetHelp(const std::string &help) {
            GetHelpOption().variable.defaultValue->DataFromStringVector({help});
            this->_autoGenHelpDocs = false;
        }

        /**
         * @brief Set the description for this program
         */
        void SetProgDescription(const std::string &progDesc) {
            _progDescription = progDesc;
        }

        /**
         * @brief set up the option parser
         *
         * @param argc the count of the argument
         * @param argv the value of the argument
         */
        void SetupFlags(int argc, char const *argv[]) {
            if (this->_autoGenVersion) {
                this->AutoGenVersion();
            }

            if (this->_autoGenHelpDocs) {
                this->AutoGenHelpDocs(argv[0]);
            }

            std::unordered_map<std::string, std::vector<std::string>> inputOptArgs;
            std::string curOptLongName = DEFAULT_OPTION_LONG_NAME;

            for (int i = 1; i != argc; ++i) {
                std::string str = argv[i];
                switch (ns_priv::IsAnOption(str)) {
                    case ns_priv::OptionNameType::OPT_SHORT_NAME:
                        if (auto optShortName = str.at(1);Utils::MapKeyExist(
                                _optShortNameToOptLongName, optShortName)) {
                            curOptLongName = _optLongNameToOpt.at(
                                    _optShortNameToOptLongName.at(optShortName)).optionLongName;
                            inputOptArgs[curOptLongName] = std::vector<std::string>();
                            // is help or version options
                            if (curOptLongName == "help") {
                                throw std::runtime_error(GetHelpOption().variable.defaultValue->ValueString());
                            } else if (curOptLongName == "version") {
                                throw std::runtime_error(
                                        std::string(argv[0]) + ": ['version': '" +
                                        GetVersionOption().variable.defaultValue->ValueString() + "']"
                                );
                            }
                        } else {
                            FLAGS_THROW_EXCEPTION(SetupFlags, "there isn't option named \"-" + optShortName + "\"");
                        }
                        break;
                    case ns_priv::OptionNameType::OPT_LONG_NAME:
                        if (auto optLongName = str.substr(2);Utils::MapKeyExist(_optLongNameToOpt, optLongName)) {
                            curOptLongName = optLongName;
                            inputOptArgs[curOptLongName] = std::vector<std::string>();
                            // is help or version options
                            if (curOptLongName == "help") {
                                throw std::runtime_error(GetHelpOption().variable.defaultValue->ValueString());
                            } else if (curOptLongName == "version") {
                                throw std::runtime_error(
                                        std::string(argv[0]) + ": ['version': '" +
                                        GetVersionOption().variable.defaultValue->ValueString() + "']"
                                );
                            }
                        } else {
                            FLAGS_THROW_EXCEPTION(SetupFlags, "there isn't option named \"--" + optLongName + "\"");
                        }
                        break;
                    case ns_priv::OptionNameType::NONE:
                        inputOptArgs[curOptLongName].push_back(str);
                        break;
                }
            }

            // the 'no-option' option is not set in the current program but user pass the 'no-option' argv(s)
            // so we need to remove it.
            if (!Utils::MapKeyExist(_optLongNameToOpt, DEFAULT_OPTION_LONG_NAME)) {
                if (Utils::MapKeyExist(inputOptArgs, DEFAULT_OPTION_LONG_NAME)) {
                    inputOptArgs.erase(DEFAULT_OPTION_LONG_NAME);
                }
            }

            // check whether any missing options have not been passed in according to the properties of the set options
            for (const auto &[optLongName, opt]: _optLongNameToOpt) {
                if (opt.property == OptionProp::OPTIONAL) {
                    continue;
                }
                if (auto iter = inputOptArgs.find(optLongName);iter == inputOptArgs.cend()) {
                    if (optLongName == DEFAULT_OPTION_LONG_NAME) {
                        FLAGS_THROW_EXCEPTION(
                                SetupFlags, "the default option is 'OptionProp::REQUIRED', but you didn't pass it"
                        );
                    } else {
                        FLAGS_THROW_EXCEPTION(
                                SetupFlags,
                                "the option named " + opt.GetOptionName() +
                                " is 'OptionProp::REQUIRED', but you didn't use it"
                        );
                    }
                }
            }

            // assert and assign
            for (const auto &[optLongName, inputArgs]: inputOptArgs) {
                auto &opt = _optLongNameToOpt.find(optLongName)->second;
                if (auto msg = opt.variable.value->DataFromStringVector(inputArgs);msg) {
                    FLAGS_THROW_EXCEPTION(
                            AssertOptionValue,
                            "the value(s) for option " + optLongName + opt.GetOptionName() +
                            " is(are) invalid: \"" + *msg + "\""
                    );
                }
                opt.AssertOptionValue();
            }
        }

        /**
         * @tparam ArgumentType the type of the option's argument
         *
         * @param optionLongName the option long name
         * @param optionShortName the option short name
         * @param defaultValue the default value of the option argument
         * @param description the description of this option
         * @param property the property of this option
         * @param assertor the assertor to judge whether the entered args are valid
         * @return the value address of the option argument
         *
         * @attention user should use reference to receive the return value
         */
        template<class ArgumentType>
        const typename ArgumentType::data_type &
        AddOption(const std::string &optionLongName, char optionShortName,
                  const typename ArgumentType::data_type &defaultValue, const std::string &description,
                  const OptionProp &property, assertor_type<ArgumentType> assertor = nullptr) {
            if (optionLongName.empty()) {
                FLAGS_THROW_EXCEPTION_DEVELOPER(
                        AddOption, "the option name shouldn't be a empty string"
                );
            }
            if (!Utils::IsALetter(optionLongName.front())) {
                FLAGS_THROW_EXCEPTION_DEVELOPER(
                        AddOption,
                        "the option long name should be capitalized. (for option \"--" + optionLongName + "\")"
                );
            }
            if (optionShortName != EMPTY_OPTION_SHORT_NAME && !Utils::IsALetter(optionShortName)) {
                FLAGS_THROW_EXCEPTION_DEVELOPER(
                        AddOption,
                        "the option short name should be capitalized. (for option \"-" + optionShortName + "\")"
                );
            }
            if (Utils::MapKeyExist(_optLongNameToOpt, optionLongName)) {
                FLAGS_THROW_EXCEPTION_DEVELOPER(
                        AddOption, "the option named \"--" + optionLongName + "\" is exists already"
                );
            }
            if (optionShortName != EMPTY_OPTION_SHORT_NAME &&
                Utils::MapKeyExist(_optShortNameToOptLongName, optionShortName)) {
                FLAGS_THROW_EXCEPTION_DEVELOPER(
                        AddOption, "the option named \"-" + optionShortName + "\" is exists already"
                );
            }
            ns_priv::Option::assertor_type tarAssertor = nullptr;
            if (assertor) {
                tarAssertor = [assertor](const std::shared_ptr<Argument> &obj) -> std::optional<std::string> {
                    return assertor(obj->template Boost<ArgumentType>()->GetData());
                };
            }
            auto option = ns_priv::Option(
                    optionLongName, optionShortName, Variable::Create<ArgumentType>(defaultValue),
                    description, property, tarAssertor
            );
            // save the option to map
            _optLongNameToOpt.insert({optionLongName, option});
            _optShortNameToOptLongName.insert({optionShortName, optionLongName});
            return _optLongNameToOpt.at(optionLongName).variable.value->template Boost<ArgumentType>()->GetData();
        }

        /**
         * @tparam ArgumentType the type of the option's argument
         *
         * @param optionLongName the option long name
         * @param defaultValue the default value of the option argument
         * @param description the description of this option
         * @param property the property of this option
         * @param assertor the assertor to judge whether the entered args are valid
         * @return the value address of the option argument
         *
         * @attention user should use reference to receive the return value
         */
        template<class ArgumentType>
        const typename ArgumentType::data_type &
        AddOption(const std::string &optLongName, const typename ArgumentType::data_type &defaultValue,
                  const std::string &description, const OptionProp &property,
                  assertor_type<ArgumentType> assertor = nullptr) {
            return AddOption < ArgumentType > (
                    optLongName, EMPTY_OPTION_SHORT_NAME, defaultValue, description, property, assertor
            );
        }

        /**
         * @tparam ArgumentType the type of the option's argument
         *
         * @param defaultValue the default value of the default option argument
         * @param description the description of this default option
         * @param property the property of this default option
         * @param assertor the assertor to judge whether the entered args are valid
         * @return the value address of the default option argument
         *
         * @attention user should use reference to receive the return value
         */
        template<class ArgumentType>
        const typename ArgumentType::data_type &
        AddDefaultOption(const typename ArgumentType::data_type &defaultValue,
                         const std::string &description, const OptionProp &property,
                         assertor_type<ArgumentType> assertor = nullptr) {
            return AddOption<ArgumentType>(DEFAULT_OPTION_LONG_NAME, defaultValue, description, property, assertor);
        }

    protected:
        ns_priv::Option GetVersionOption() { return _optLongNameToOpt.at("version"); }

        ns_priv::Option GetHelpOption() { return _optLongNameToOpt.at("help"); }

        void AutoGenHelpDocs(const std::string &programName) {
            std::stringstream stream;

            // program description
            if (!_progDescription.empty()) {
                stream << _progDescription << "\n\n";
            }

            // the main usage of this program
            stream << "Usage: " << programName;
            if (Utils::MapKeyExist(_optLongNameToOpt, DEFAULT_OPTION_LONG_NAME)) {
                stream << " [def-opt argv(s)]";
            }
            stream << " [opt-name argv(s)] ...\n\n";

            // the header of the help docs
            stream << "    " << std::setw(15) << std::left << "Options"
                   << std::setw(15) << std::left << "Property"
                   << std::setw(15) << std::left << "Type"
                   << "Describes\n";
            stream << std::string(62, '-') << '\n';

            if (Utils::MapKeyExist(_optLongNameToOpt, DEFAULT_OPTION_LONG_NAME)) {
                auto &nopt = _optLongNameToOpt.at(DEFAULT_OPTION_LONG_NAME);
                stream << "  --" << std::setw(15) << std::left << "def-opt"
                       << std::setw(15) << std::left << nopt.property
                       << std::setw(15) << std::left << nopt.variable.defaultValue->TypeNameString()
                       << nopt.description << "\n\n";
            }

            for (const auto &elem: _optLongNameToOpt) {
                if (elem.first == "help" || elem.first == "version" || elem.first == DEFAULT_OPTION_LONG_NAME) {
                    continue;
                }
                if (elem.second.optionShortName != EMPTY_OPTION_SHORT_NAME) {
                    stream << "  --" << std::setw(15) << std::left
                           << elem.second.optionLongName + ", -" + elem.second.optionShortName;
                } else {
                    stream << "  --" << std::setw(15) << std::left << elem.second.optionLongName;
                }
                stream << std::setw(15) << std::left << elem.second.property
                       << std::setw(15) << std::left << elem.second.variable.defaultValue->TypeNameString()
                       << elem.second.description << '\n';
            }

            // help and version
            auto &help = _optLongNameToOpt.at("help");
            auto &version = _optLongNameToOpt.at("version");
            stream << "\n  --" << std::setw(15) << std::left << help.optionLongName + ", -" + help.optionShortName
                   << std::setw(15) << std::left << help.property
                   << std::setw(15) << std::left << help.variable.defaultValue->TypeNameString()
                   << help.description;
            stream << "\n  --" << std::setw(15) << std::left << version.optionLongName + ", -" + version.optionShortName
                   << std::setw(15) << std::left << version.property
                   << std::setw(15) << std::left << version.variable.defaultValue->TypeNameString()
                   << version.description;

            // suffix
            stream << "\n\nhelp docs for program \"" + programName + "\"";

            // assign
            GetHelpOption().variable.defaultValue->DataFromStringVector({stream.str()});
        }

        void AutoGenVersion() {
            GetVersionOption().variable.defaultValue->DataFromStringVector({"1.0.0"});
        }

    private:

        void CreateMapForShortLongOptionName(const ns_priv::Option &option) {
            _optShortNameToOptLongName.insert({option.optionShortName, option.optionLongName});
        }

        /**
         * @brief override operator '<<' for type 'Option'
         */
        friend std::ostream &operator<<(std::ostream &os, const OptionParser &obj) {
            for (const auto &[optionName, option]: obj.GetOptionMap()) {
                if (optionName == "help" || optionName == "version") {
                    auto tmpOption = option;
                    tmpOption.variable.defaultValue->DataFromStringVector({"\"...\""});
                    tmpOption.variable.value->DataFromStringVector({"\"...\""});
                    os << option << std::endl;
                } else {
                    os << option << std::endl;
                }
            }
            return os;
        }
    } parser;

#undef FLAGS_THROW_EXCEPTION
#undef FLAGS_THROW_EXCEPTION_DEVELOPER
}


#endif