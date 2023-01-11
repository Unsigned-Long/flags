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
        std::shared_ptr<Argument> defaultValue, Value;
    };

    namespace ns_priv {
        struct Option {
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
            std::function<void(const std::shared_ptr<Argument> &)> assertor;

        public:
            /**
             * @brief Construct a new Option object
             */
            Option(std::string optionName, std::string variableName, Variable variable,
                   std::string description, const OptionProp &property)
                    : optionName(std::move(optionName)), variableName(std::move(variableName)),
                      variable(std::move(variable)), description(std::move(description)), property(property) {}

            ~Option() = default;

            /**
             * @brief assertor for the variable's value
             */
            void AssertVariable() const {
                if (this->assertor != nullptr) {
                    this->assertor(this->variable.Value);
                }
            }

            /**
             * @brief override operator '<<' for type 'Option'
             */
            friend std::ostream &operator<<(std::ostream &os, const Option &obj) {
                os << '{';
                os << "'optionName': \"" << obj.optionName << "\", ";
                os << "'variableName': \"" << obj.variableName << "\", ";
                os << "'DefaultValue': " << obj.variable.defaultValue->ValueString() << ", ";
                os << "'Value': " << obj.variable.Value->ValueString() << ", ";
                os << "'desc': \"" << obj.description << "\", ";
                os << "'prop': " << obj.property << ", ";
                os << "'argType': " << obj.variable.defaultValue->TypeNameString();
                os << '}';
                return os;
            };
        };
    }
}


#endif