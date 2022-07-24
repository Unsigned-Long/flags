/**
 * @file flags-v2.hpp
 * @author shlchen (3079625093@qq.com)
 * @brief the second version for lib-flags
 * @version 0.1
 * @date 2022-05-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef FLAGS_V2_H
#define FLAGS_V2_H

#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ns_flags {
  /**
   * @brief throw exceptions for this lib
   */
#define _FLAG_THROW_EXCEPTION_(where, msg)                                               \
  throw std::runtime_error(std::string("[ error from 'lib-flags':'") + #where + "' ] " + \
                           msg + ". (use option '--help' to get more info)")

#define _FLAG_THROW_EXCEPTION_DEV_(where, msg)                                                        \
  throw std::runtime_error(std::string("[ error from 'lib-flags':'") + #where + "' to developer ] " + \
                           msg + ".")

  /**
   * @brief Properties of options
   */
  enum class OptionProp {
    // the option is optional
    OPTIONAL,
    // the option is required, and it's necessary
    REQUIRED
  };

  /**
   * @brief override operator '<<' for type 'OptionProp'
   */
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
  };

  namespace ns_priv {
    // the type of argument
    enum class ArgType {
      /**
       * @brief options
       */
      INT,
      DOUBLE,
      FLOAT,
      BOOL,
      STRING,
      INT_VEC,
      DOUBLE_VEC,
      FLOAT_VEC,
      BOOL_VEC,
      STRING_VEC,
      // special types
      HELP,
      VERSION
    };

    /**
     * @brief override operator '<<' for type 'ArgType'
     */
    static std::ostream &operator<<(std::ostream &os, const ArgType &obj) {
      switch (obj) {
      case ArgType::INT:
        os << "INT";
        break;
      case ArgType::DOUBLE:
        os << "DOUBLE";
        break;
      case ArgType::FLOAT:
        os << "FLOAT";
        break;
      case ArgType::BOOL:
        os << "BOOL";
        break;
      case ArgType::STRING:
        os << "STRING";
        break;
      case ArgType::INT_VEC:
        os << "INT_VEC";
        break;
      case ArgType::DOUBLE_VEC:
        os << "DOUBLE_VEC";
        break;
      case ArgType::FLOAT_VEC:
        os << "FLOAT_VEC";
        break;
      case ArgType::BOOL_VEC:
        os << "BOOL_VEC";
        break;
      case ArgType::STRING_VEC:
        os << "STRING_VEC";
        break;
      case ArgType::HELP:
        os << "HELP";
        break;
      case ArgType::VERSION:
        os << "VERSION";
        break;
      }
      return os;
    };

    struct Option {
    public:
      // option's name
      const std::string _optName;
      // variable's name
      const std::string _varName;
      // the default value
      void *_varDefault;
      // the variable
      void *_var;
      // to describe this option
      const std::string _desc;
      // the properties of this option
      const OptionProp _prop;
      // the type of this variable
      const ArgType _argType;
      // assert function
      std::function<void(void *)> _assert;

      /**
       * @brief Construct a new Option object
       */
      Option(std::string optName, std::string varName, void *varDefault, void *var,
             std::string desc, const OptionProp &prop, const ArgType argType)
          : _optName(std::move(optName)), _varName(std::move(varName)), _varDefault(varDefault), _var(var),
            _desc(std::move(desc)), _prop(prop), _argType(argType) {}

      ~Option() {
        // delete the variable's default value pointer
        // before that, cast the (void *) type to target type
        // it's unnecessary for 'HELP' and 'VERSION' options
        switch (this->_argType) {
        case ArgType::INT:
          delete ((int *)(this->_varDefault));
          break;
        case ArgType::INT_VEC:
          delete ((std::vector<int> *)(this->_varDefault));
          break;
        case ArgType::DOUBLE:
          delete ((double *)(this->_varDefault));
          break;
        case ArgType::DOUBLE_VEC:
          delete ((std::vector<double> *)(this->_varDefault));
          break;
        case ArgType::FLOAT:
          delete ((float *)(this->_varDefault));
          break;
        case ArgType::FLOAT_VEC:
          delete ((std::vector<float> *)(this->_varDefault));
          break;
        case ArgType::BOOL:
          delete ((bool *)(this->_varDefault));
          break;
        case ArgType::BOOL_VEC:
          delete ((std::vector<bool> *)(this->_varDefault));
          break;
        case ArgType::STRING:
          delete ((std::string *)(this->_varDefault));
          break;
        case ArgType::STRING_VEC:
          delete ((std::vector<std::string> *)(this->_varDefault));
          break;
        case ArgType::HELP:
          delete ((std::string *)(this->_varDefault));
          break;
        case ArgType::VERSION:
          delete ((std::string *)(this->_varDefault));
          break;
        default:
          break;
        }
      }

      /**
       * @brief assert for the variable's value
       */
      void assertVar() {
        if (this->_assert != nullptr) {
          this->_assert(this->_var);
        }
      }

      /**
       * @brief assign the variable using value
       */
      template <typename Type>
      void assignVar(const Type &val) {
        *((Type *)(this->_var)) = val;
      }

      /**
       * @brief print a vector
       */
      template <typename ElemType>
      static void printVec(std::ostream &os, const std::vector<ElemType> &vec) {
        os << '[';
        if (vec.empty()) {
          os << ']';
          return;
        }
        for (int i = 0; i != vec.size() - 1; ++i) {
          os << vec.at(i) << ", ";
        }
        os << vec.back();
        os << ']';
      }
    };

    /**
     * @brief override operator '<<' for type 'Option'
     */
    static std::ostream &operator<<(std::ostream &os, const Option &obj) {
      os << '{';
      os << "'optName': \"" << obj._optName << "\", ";
      os << "'varName': \"" << obj._varName << "\", ";
      switch (obj._argType) {
      case ArgType::INT:
        os << "'varDefVal': " << *((int *)(obj._varDefault)) << ", ";
        os << "'varVal': " << *((int *)(obj._var)) << ", ";
        break;
      case ArgType::INT_VEC:
        os << "'varDefVal': ";
        Option::printVec(os, *((std::vector<int> *)(obj._varDefault)));
        os << ", 'varVal': ";
        Option::printVec(os, *((std::vector<int> *)(obj._var)));
        os << ", ";
        break;
      case ArgType::DOUBLE:
        os << "'varDefVal': " << *((double *)(obj._varDefault)) << ", ";
        os << "'varVal': " << *((double *)(obj._var)) << ", ";
        break;
      case ArgType::DOUBLE_VEC:
        os << "'varDefVal': ";
        Option::printVec(os, *((std::vector<double> *)(obj._varDefault)));
        os << ", 'varVal': ";
        Option::printVec(os, *((std::vector<double> *)(obj._var)));
        os << ", ";
        break;
      case ArgType::FLOAT:
        os << "'varDefVal': " << *((float *)(obj._varDefault)) << ", ";
        os << "'varVal': " << *((float *)(obj._var)) << ", ";
        break;
      case ArgType::FLOAT_VEC:
        os << "'varDefVal': ";
        Option::printVec(os, *((std::vector<float> *)(obj._varDefault)));
        os << ", 'varVal': ";
        Option::printVec(os, *((std::vector<float> *)(obj._var)));
        os << ", ";
        break;
      case ArgType::BOOL:
        os << "'varDefVal': " << *((bool *)(obj._varDefault)) << ", ";
        os << "'varVal': " << *((bool *)(obj._var)) << ", ";
        break;
      case ArgType::BOOL_VEC:
        os << "'varDefVal': ";
        Option::printVec(os, *((std::vector<bool> *)(obj._varDefault)));
        os << ", 'varVal': ";
        Option::printVec(os, *((std::vector<bool> *)(obj._var)));
        os << ", ";
        break;
      case ArgType::STRING:
        os << "'varDefVal': \"" << *((std::string *)(obj._varDefault)) << "\", ";
        os << "'varVal': \"" << *((std::string *)(obj._var)) << "\", ";
        break;
      case ArgType::STRING_VEC:
        os << "'varDefVal': ";
        Option::printVec(os, *((std::vector<std::string> *)(obj._varDefault)));
        os << ", 'varVal': ";
        Option::printVec(os, *((std::vector<std::string> *)(obj._var)));
        os << ", ";
        break;
      case ArgType::HELP:
        os << "'value': \"...\", ";
        break;
      case ArgType::VERSION:
        os << "'value': \"" << *((std::string *)(obj._varDefault)) << "\", ";
        break;
      default:
        break;
      }
      os << "'desc': \"" << obj._desc << "\", ";
      os << "'prop': " << obj._prop << ", ";
      os << "'argType': " << obj._argType;
      os << '}';
      return os;
    };

    // Inherited from std::unordered_map, it stores the option information set by the user in advance
    class OptionParser : public std::unordered_map<std::string, std::shared_ptr<ns_priv::Option>> {
    public:
      using parent_type = std::unordered_map<std::string, std::shared_ptr<ns_priv::Option>>;
      using parent_type::parent_type;

    private:
      // Determine whether to automatically generate corresponding content
      // If the user explicitly sets the corresponding content, we refuse to generate it automatically
      bool _autoGenHelpDocs;
      bool _autoGenVersion;
      const std::string __NOPT__ = "__NOPT__";

    public:
      OptionParser() : _autoGenHelpDocs(true), _autoGenVersion(true) {
        // add help and version options
        // Note that the address corresponding to the variable is nullptr
        this->insert({"help", std::make_shared<Option>("help", "null", (void *)(new std::string("")), nullptr,
                                                       "display the help docs", OptionProp::OPTIONAL, ArgType::HELP)});
        this->insert({"version", std::make_shared<Option>("version", "null", (void *)(new std::string("")), nullptr,
                                                          "display the version of this program", OptionProp::OPTIONAL, ArgType::VERSION)});
      }

      /**
       * @brief Set the Version
       */
      void setVersion(const std::string &version) {
        (*(std::string *)(this->at("version")->_varDefault)) = version;
        this->_autoGenVersion = false;
      }

      /**
       * @brief Set the Help Docs
       */
      void setHelpDocs(const std::string &help) {
        (*(std::string *)(this->at("help")->_varDefault)) = help;
        this->_autoGenHelpDocs = false;
      }

      /**
       * @brief set up the option parser
       *
       * @param argc the count of the argument
       * @param argv the value of the argument
       */
      void setupFlags(int argc, char const *argv[]) {
        if (this->_autoGenVersion) {
          this->autoGenVersion();
        }

        if (this->_autoGenHelpDocs) {
          this->autoGenHelpDocs(argv[0]);
        }

        std::map<std::string, std::vector<std::string>> inputArgs;
        std::vector<std::string> optNames;
        std::string curOption = __NOPT__;

        for (int i = 1; i != argc; ++i) {
          std::string str = argv[i];
          if (ns_flags::ns_priv::OptionParser::isAnOption(str)) {
            curOption = str.substr(2);
            inputArgs[curOption] = std::vector<std::string>();
            optNames.push_back(curOption);
            // is help or version options
            if (curOption == "help") {
              throw std::runtime_error((*(std::string *)(this->at("help")->_varDefault)));
            } else if (curOption == "version") {
              throw std::runtime_error(std::string(argv[0]) + ": ['version': '" +
                                       (*(std::string *)(this->at("version")->_varDefault)) + "']");
            }
          } else {
            inputArgs[curOption].push_back(str);
          }
        }

        // the 'no-option' option is not set in the current program but user pass the 'no-option' argv(s)
        // so we need to remove it.
        if (this->find(__NOPT__) == this->cend()) {
          if (inputArgs.find(__NOPT__) != inputArgs.cend()) {
            inputArgs.erase(__NOPT__);
          }
        }

        // check invalid options
        for (const auto &optName : optNames) {
          if (!this->isAValidOption(optName)) {
            _FLAG_THROW_EXCEPTION_(setupFlags, "there isn't option named '--" + optName + "'");
          }
        }

        // check whether any missing options have not been passed in according to the properties of the set options
        for (const auto &[optName, opt] : *this) {
          if (opt->_prop == OptionProp::OPTIONAL) {
            continue;
          }
          if (inputArgs.find(optName) == inputArgs.cend()) {
            if (optName == __NOPT__) {
              _FLAG_THROW_EXCEPTION_(setupFlags, "the 'no-option' argv is 'OptionProp::REQUIRED', but you didn't pass it");
            } else {
              _FLAG_THROW_EXCEPTION_(setupFlags, "the option named '--" + optName + "' is 'OptionProp::REQUIRED', but you didn't use it");
            }
          }
        }

        // assert and assign
        for (const auto &[optName, inputOpt] : inputArgs) {
          auto &opt = *this->find(optName)->second;

          switch (opt._argType) {
          case ArgType::INT:
            if (!inputOpt.empty()) {
              opt.assignVar<int>(std::stoi(inputOpt.front()));
            }
            break;
          case ArgType::INT_VEC:
            if (!inputOpt.empty()) {
              std::vector<int> vec;
              for (const auto &elem : inputOpt) {
                vec.push_back(std::stoi(elem));
              }
              opt.assignVar<std::vector<int>>(vec);
            }
            break;
          case ArgType::DOUBLE:
            if (!inputOpt.empty()) {
              opt.assignVar<double>(std::stod(inputOpt.front()));
            }
            break;
          case ArgType::DOUBLE_VEC:
            if (!inputOpt.empty()) {
              std::vector<double> vec;
              for (const auto &elem : inputOpt) {
                vec.push_back(std::stod(elem));
              }
              opt.assignVar<std::vector<double>>(vec);
            }
            break;
          case ArgType::FLOAT:
            if (!inputOpt.empty()) {
              opt.assignVar<float>(std::stof(inputOpt.front()));
            }
            break;
          case ArgType::FLOAT_VEC:
            if (!inputOpt.empty()) {
              std::vector<float> vec;
              for (const auto &elem : inputOpt) {
                vec.push_back(std::stof(elem));
              }
              opt.assignVar<std::vector<float>>(vec);
            }
            break;
          case ArgType::BOOL:
            if (inputOpt.empty()) {
              opt.assignVar<bool>(true);
            } else {
              opt.assignVar<bool>(ns_flags::ns_priv::OptionParser::toBool(inputOpt.front()));
            }
            break;
          case ArgType::BOOL_VEC:
            if (!inputOpt.empty()) {
              std::vector<bool> vec;
              for (const auto &elem : inputOpt) {
                vec.push_back(ns_flags::ns_priv::OptionParser::toBool(elem));
              }
              opt.assignVar<std::vector<bool>>(vec);
            }
            break;
          case ArgType::STRING:
            if (!inputOpt.empty()) {
              opt.assignVar<std::string>(inputOpt.front());
            }
            break;
          case ArgType::STRING_VEC:
            if (!inputOpt.empty()) {
              opt.assignVar<std::vector<std::string>>(inputOpt);
            }
            break;
          case ArgType::HELP:
            break;
          case ArgType::VERSION:
            break;
          default:
            break;
          }

          opt.assertVar();
        }
      }

    protected:
      void autoGenHelpDocs(const std::string &programName) {
        std::stringstream stream;

        // the main usage of this program
        stream << "Usage: " << programName;
        if (this->find(__NOPT__) != this->cend()) {
          stream << " [no-opt argv(s)]";
        }
        stream << " [--optName argv(s)] ...\n\n";

        // the header of the help docs
        stream << "    " << std::setw(15) << std::left << "Options"
               << std::setw(15) << std::left << "Property"
               << std::setw(15) << std::left << "Type"
               << "Describes\n";
        stream << std::string(62, '-') << '\n';

        if (this->find(__NOPT__) != this->cend()) {
          auto &nopt = this->at(__NOPT__);
          stream << "  --" << std::setw(15) << std::left << "no-opt"
                 << std::setw(15) << std::left << nopt->_prop
                 << std::setw(15) << std::left << nopt->_argType
                 << nopt->_desc << "\n\n";
        }

        for (const auto &elem : *this) {
          if (elem.first == "help" || elem.first == "version" || elem.first == __NOPT__) {
            continue;
          }
          stream << "  --" << std::setw(15) << std::left << elem.second->_optName
                 << std::setw(15) << std::left << elem.second->_prop
                 << std::setw(15) << std::left << elem.second->_argType
                 << elem.second->_desc << '\n';
        }

        // help and version
        auto &help = this->at("help");
        auto &version = this->at("version");
        stream << "\n  --" << std::setw(15) << std::left << help->_optName
               << std::setw(15) << std::left << help->_prop
               << std::setw(15) << std::left << help->_argType
               << help->_desc;
        stream << "\n  --" << std::setw(15) << std::left << version->_optName
               << std::setw(15) << std::left << version->_prop
               << std::setw(15) << std::left << version->_argType
               << version->_desc;

        // suffix
        stream << "\n\nhelp docs for program \"" + programName + "\"";

        // assign
        (*(std::string *)(this->at("help")->_varDefault)) = stream.str();
      }

      void autoGenVersion() {
        (*(std::string *)(this->at("version")->_varDefault)) = "1.0.0";
      }

      /**
       * @brief Judge whether a parameter is an option name
       *
       * @param str the argv
       * @return true
       * @return false
       */
      static bool isAnOption(const std::string &str) {
        return str.size() > 3 && str.substr(0, 2) == "--";
      }

      /**
       * @brief Judge whether an option is valid (whether it has been set)
       *
       * @param str the option name
       * @return true
       * @return false
       */
      bool isAValidOption(const std::string &str) {
        return this->find(str) != this->cend();
      }

      /**
       * @brief Converts a string to lowercase
       *
       * @param str the string
       * @return std::string
       */
      static std::string tolower(const std::string &str) {
        std::string lowerStr;
        for (char i : str) {
          lowerStr.push_back(std::tolower(i));
        }
        return lowerStr;
      }

      /**
       * @brief Convert a string to Boolean
       *
       * @param str the string
       */
      static bool toBool(const std::string &str) {
        std::string val = ns_flags::ns_priv::OptionParser::tolower(str);
        bool b;
        if (val == "on" || val == "1" || val == "true") {
          b = true;
        } else if (val == "off" || val == "0" || val == "false") {
          b = false;
        } else if (val.empty()) {
          b = true;
        } else {
          b = std::stoi(str);
        }
        return b;
      }
    } parser;
    /**
     * @brief override operator '<<' for type 'Option'
     */
    static std::ostream &operator<<(std::ostream &os, const OptionParser &obj) {
      for (const auto &elem : obj) {
        os << *elem.second << std::endl;
      }
      return os;
    }

  } // namespace ns_priv

#define FLAGS_INSERT_OPTION(optName, varName, varDefault, var, desc, prop, argType) \
  ns_flags::ns_priv::parser                                                      \
      .insert({optName, std::make_shared<ns_flags::ns_priv::Option>(optName, #varName, varDefault, var, desc, prop, argType)});

#define FLAGS_DEF(builtInType, argType, varName, optName, desc, prop, ...)                   \
  builtInType flags_##varName = builtInType{__VA_ARGS__};                                    \
  FLAGS_INSERT_OPTION(#optName, flags_##varName, (void *)(new builtInType(flags_##varName)), \
                      (void *)(&flags_##varName), desc, prop,                                \
                      argType);

  /**
   * @attention Here are the functions and macros you will use.
   */

  static void setVersion(const std::string &version) {
    ns_priv::parser.setVersion(version);
  }

  static void setHelpDocs(const std::string &help) {
    ns_priv::parser.setHelpDocs(help);
  }

  static void setupFlags(int argc, char const *argv[]) {
    return ns_priv::parser.setupFlags(argc, argv);
  }

/**
 * Macros for adding options for different parameter types
 */
#define FLAGS_DEF_INT(varName, optName, desc, prop, ...) \
  FLAGS_DEF(int, ns_flags::ns_priv::ArgType::INT, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_INT_VEC(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::vector<int>, ns_flags::ns_priv::ArgType::INT_VEC, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_FLOAT(varName, optName, desc, prop, ...) \
  FLAGS_DEF(float, ns_flags::ns_priv::ArgType::FLOAT, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_FLOAT_VEC(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::vector<float>, ns_flags::ns_priv::ArgType::FLOAT_VEC, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_DOUBLE(varName, optName, desc, prop, ...) \
  FLAGS_DEF(double, ns_flags::ns_priv::ArgType::DOUBLE, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_DOUBLE_VEC(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::vector<double>, ns_flags::ns_priv::ArgType::DOUBLE_VEC, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_BOOL(varName, optName, desc, prop, ...) \
  FLAGS_DEF(bool, ns_flags::ns_priv::ArgType::BOOL, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_BOOL_VEC(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::vector<bool>, ns_flags::ns_priv::ArgType::BOOL_VEC, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_STRING(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::string, ns_flags::ns_priv::ArgType::STRING, varName, optName, desc, prop, __VA_ARGS__)

#define FLAGS_DEF_STRING_VEC(varName, optName, desc, prop, ...) \
  FLAGS_DEF(std::vector<std::string>, ns_flags::ns_priv::ArgType::STRING_VEC, varName, optName, desc, prop, __VA_ARGS__)
/**
 * Define macros that do not require option parameters
 */
#define FLAGS_DEF_NO_OPTION(type, varName, desc, prop, ...) \
  FLAGS_DEF_##type(varName, __NOPT__, desc, prop, __VA_ARGS__)

/**
 * Developers can judge the input value by adding assertion function to the option to see whether it is valid
 * If the option to add an assertion function is not set in advance, an exception will be thrown to the developer
 */
#define FLAGS_ASSERT(builtInType, optName, invaildMsg, fun)                                                                \
  if (ns_flags::ns_priv::parser.find(#optName) == ns_flags::ns_priv::parser.cend()) {                                \
    _FLAG_THROW_EXCEPTION_DEV_(FLAGS_SET_ASSERT,                                                                           \
                               "you cannot add an assertion function for an undefined option named '--" + #optName + "'"); \
  } else {                                                                                                                 \
    ns_flags::ns_priv::parser.find(#optName)->second->_assert = [](void *val) -> void {                                 \
      if (!(fun(*(builtInType *)(val)))) {                                                                                 \
        _FLAG_THROW_EXCEPTION_(FLAGS_SET_ASSERT,                                                                           \
                               "the value for option '--" + #optName + "' is invalid: \"" + invaildMsg + "\"");            \
      }                                                                                                                    \
    };                                                                                                                     \
  }

/**
 * Define assertion function macros for different type
 */
#define FLAGS_ASSERT_INT(optName, invaildMsg, fun) \
  FLAGS_ASSERT(int, optName, invaildMsg, fun)

#define FLAGS_ASSERT_INT_VEC(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::vector<int>, optName, invaildMsg, fun)

#define FLAGS_ASSERT_FLOAT(optName, invaildMsg, fun) \
  FLAGS_ASSERT(float, optName, invaildMsg, fun)

#define FLAGS_ASSERT_FLOAT_VEC(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::vector<float>, optName, invaildMsg, fun)

#define FLAGS_ASSERT_DOUBLE(optName, invaildMsg, fun) \
  FLAGS_ASSERT(double, optName, invaildMsg, fun)

#define FLAGS_ASSERT_DOUBLE_VEC(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::vector<double>, optName, invaildMsg, fun)

#define FLAGS_ASSERT_BOOL(optName, invaildMsg, fun) \
  FLAGS_ASSERT(bool, optName, invaildMsg, fun)

#define FLAGS_ASSERT_BOOL_VEC(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::vector<bool>, optName, invaildMsg, fun)

#define FLAGS_ASSERT_STRING(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::string, optName, invaildMsg, fun)

#define FLAGS_ASSERT_STRING_VEC(optName, invaildMsg, fun) \
  FLAGS_ASSERT(std::vector<std::string>, optName, invaildMsg, fun)

} // namespace ns_flags

#endif