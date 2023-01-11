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
        struct Option {
        public:
            using assertor_type = std::function<std::optional<std::string>(const std::shared_ptr<Argument> &)>;

        public:
            // option's name
            const std::string optionName;

            // the value
            Variable variable;

            // to describe this option
            const std::string description;
            // the properties of this option
            const OptionProp property;
            // assertor
            assertor_type assertor;

        public:
            /**
             * @brief Construct a new Option object
             */
            Option(std::string optionName, Variable variable,
                   std::string description, const OptionProp &property, assertor_type assertor = nullptr)
                    : optionName(std::move(optionName)), variable(std::move(variable)),
                      description(std::move(description)), property(property), assertor(std::move(assertor)) {}

            ~Option() = default;

            /**
             * @brief assertor for the variable's value
             */
            void AssertOptionValue() const {
                if (this->assertor == nullptr) { return; }

                if (auto msg = this->assertor(this->variable.value);msg) {
                    FLAGS_THROW_EXCEPTION(
                            AssertOptionValue,
                            "the value(s) for option '--" + optionName + "' is(are) invalid: \"" + *msg + "\""
                    );
                }
            }

            /**
             * @brief override operator '<<' for type 'Option'
             */
            friend std::ostream &operator<<(std::ostream &os, const Option &obj) {
                os << std::boolalpha;
                os << '{';
                os << "'optionName': \"" << obj.optionName << "\", ";
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

    class OptionParser : public std::unordered_map<std::string, ns_priv::Option> {
    public:
        using parent_type = std::unordered_map<std::string, ns_priv::Option>;
        using parent_type::parent_type;
        template<class ArgumentType>
        using assertor_type = std::function<std::optional<std::string>(const typename ArgumentType::data_type &)>;

    private:
        // Determine whether to automatically generate corresponding content
        // If the user explicitly sets the corresponding content, we refuse to generate it automatically
        bool _autoGenHelpDocs;
        bool _autoGenVersion;
        const std::string DEFAULT_OPTION_NAME = "def-opt";

    public:
        OptionParser() : _autoGenHelpDocs(true), _autoGenVersion(true) {
            // add help and version options
            // Note that the address corresponding to the variable is nullptr

            auto versionOption = ns_priv::Option(
                    "version", Variable::Create<Version>(""), "display the version of this program",
                    OptionProp::OPTIONAL, nullptr
            );
            this->insert({versionOption.optionName, versionOption});

            auto helpOption = ns_priv::Option(
                    "help", Variable::Create<Help>(""), "display the help docs",
                    OptionProp::OPTIONAL, nullptr
            );
            this->insert({helpOption.optionName, helpOption});
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

            std::map<std::string, std::vector<std::string>> inputArgs;
            std::vector<std::string> optNames;
            std::string curOption = DEFAULT_OPTION_NAME;

            for (int i = 1; i != argc; ++i) {
                std::string str = argv[i];
                if (IsAnOption(str)) {
                    curOption = str.substr(2);
                    inputArgs[curOption] = std::vector<std::string>();
                    optNames.push_back(curOption);
                    // is help or version options
                    if (curOption == "help") {
                        throw std::runtime_error(GetHelpOption().variable.defaultValue->ValueString());
                    } else if (curOption == "version") {
                        throw std::runtime_error(
                                std::string(argv[0]) + ": ['version': '" +
                                GetVersionOption().variable.defaultValue->ValueString() + "']"
                        );
                    }
                } else {
                    inputArgs[curOption].push_back(str);
                }
            }

            // the 'no-option' option is not set in the current program but user pass the 'no-option' argv(s)
            // so we need to remove it.
            if (this->find(DEFAULT_OPTION_NAME) == this->cend()) {
                if (inputArgs.find(DEFAULT_OPTION_NAME) != inputArgs.cend()) {
                    inputArgs.erase(DEFAULT_OPTION_NAME);
                }
            }

            // check invalid options
            for (const auto &optName: optNames) {
                if (!this->IsValidOption(optName)) {
                    FLAGS_THROW_EXCEPTION(SetupFlags, "there isn't option named '--" + optName + "'");
                }
            }

            // check whether any missing options have not been passed in according to the properties of the set options
            for (const auto &[optName, opt]: *this) {
                if (opt.property == OptionProp::OPTIONAL) {
                    continue;
                }
                auto iter = inputArgs.find(optName);
                if (iter == inputArgs.cend()) {
                    if (optName == DEFAULT_OPTION_NAME) {
                        FLAGS_THROW_EXCEPTION(
                                SetupFlags, "the default option is 'OptionProp::REQUIRED', but you didn't pass it"
                        );
                    } else {
                        FLAGS_THROW_EXCEPTION(
                                SetupFlags,
                                "the option named '--" + optName + "' is 'OptionProp::REQUIRED', but you didn't use it"
                        );
                    }
                } else if (iter->second.empty()) {
                    FLAGS_THROW_EXCEPTION(
                            SetupFlags, "the option named '--" + optName +
                                        "' is 'OptionProp::REQUIRED', you should pass some arguments to it"
                    );
                }
            }

            // assert and assign
            for (const auto &[optName, inputOpt]: inputArgs) {
                auto &opt = this->find(optName)->second;
                opt.variable.value->DataFromStringVector(inputOpt);
                opt.AssertOptionValue();
            }
        }

        /**
         * @tparam ArgumentType the type of the option's argument
         *
         * @param optionName the option name
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
        AddOption(const std::string &optionName, const typename ArgumentType::data_type &defaultValue,
                  const std::string &description, const OptionProp &property,
                  assertor_type<ArgumentType> assertor = nullptr) {
            ns_priv::Option::assertor_type tarAssertor = nullptr;
            if (assertor) {
                tarAssertor = [assertor](const std::shared_ptr<Argument> &obj) -> std::optional<std::string> {
                    return assertor(obj->template Boost<ArgumentType>()->GetData());
                };
            }
            auto option = ns_priv::Option(
                    optionName, Variable::Create<ArgumentType>(defaultValue), description, property, tarAssertor
            );
            this->insert({optionName, option});
            return this->at(optionName).variable.value->template Boost<ArgumentType>()->GetData();
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
            return AddOption<ArgumentType>(DEFAULT_OPTION_NAME, defaultValue, description, property, assertor);
        }

    protected:
        ns_priv::Option GetVersionOption() { return this->at("version"); }

        ns_priv::Option GetHelpOption() { return this->at("help"); }

        void AutoGenHelpDocs(const std::string &programName) {
            std::stringstream stream;

            // the main usage of this program
            stream << "Usage: " << programName;
            if (this->find(DEFAULT_OPTION_NAME) != this->cend()) {
                stream << " [def-opt argv(s)]";
            }
            stream << " [--optName argv(s)] ...\n\n";

            // the header of the help docs
            stream << "    " << std::setw(15) << std::left << "Options"
                   << std::setw(15) << std::left << "Property"
                   << std::setw(15) << std::left << "Type"
                   << "Describes\n";
            stream << std::string(62, '-') << '\n';

            if (this->find(DEFAULT_OPTION_NAME) != this->cend()) {
                auto &nopt = this->at(DEFAULT_OPTION_NAME);
                stream << "  --" << std::setw(15) << std::left << "def-opt"
                       << std::setw(15) << std::left << nopt.property
                       << std::setw(15) << std::left << nopt.variable.defaultValue->TypeNameString()
                       << nopt.description << "\n\n";
            }

            for (const auto &elem: *this) {
                if (elem.first == "help" || elem.first == "version" || elem.first == DEFAULT_OPTION_NAME) {
                    continue;
                }
                stream << "  --" << std::setw(15) << std::left << elem.second.optionName
                       << std::setw(15) << std::left << elem.second.property
                       << std::setw(15) << std::left << elem.second.variable.defaultValue->TypeNameString()
                       << elem.second.description << '\n';
            }

            // help and version
            auto &help = this->at("help");
            auto &version = this->at("version");
            stream << "\n  --" << std::setw(15) << std::left << help.optionName
                   << std::setw(15) << std::left << help.property
                   << std::setw(15) << std::left << help.variable.defaultValue->TypeNameString()
                   << help.description;
            stream << "\n  --" << std::setw(15) << std::left << version.optionName
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
        /**
         * @brief Judge whether a parameter is an option name
         *
         * @param str the argv
         * @return true
         * @return false
         */
        static bool IsAnOption(const std::string &str) {
            return str.size() > 2 && str.substr(0, 2) == "--";
        }

        /**
         * @brief Judge whether an option is valid (whether it has been set)
         *
         * @param str the option name
         * @return true
         * @return false
         */
        bool IsValidOption(const std::string &str) {
            return this->find(str) != this->cend();
        }

        /**
         * @brief override operator '<<' for type 'Option'
         */
        friend std::ostream &operator<<(std::ostream &os, const OptionParser &obj) {
            for (const auto &[optionName, option]: obj) {
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
}


#endif