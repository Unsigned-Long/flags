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

#include <utility>
#include <ostream>

#include "arg_type.hpp"
#include "exception"

namespace ns_flags {
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
            // variable's name
            const std::string variableName;

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
            Option(std::string optionName, std::string variableName, Variable variable,
                   std::string description, const OptionProp &property, assertor_type assertor = nullptr)
                    : optionName(std::move(optionName)), variableName(std::move(variableName)),
                      variable(std::move(variable)), description(std::move(description)),
                      property(property), assertor(std::move(assertor)) {}

            ~Option() = default;

            /**
             * @brief assertor for the variable's value
             */
            void AssertVariable() const {
                if (this->assertor != nullptr) {
                    this->assertor(this->variable.value);
                }
            }

            /**
             * @brief override operator '<<' for type 'Option'
             */
            friend std::ostream &operator<<(std::ostream &os, const Option &obj) {
                os << std::boolalpha;
                os << '{';
                os << "'optionName': \"" << obj.optionName << "\", ";
                os << "'variableName': \"" << obj.variableName << "\", ";
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
}


#endif