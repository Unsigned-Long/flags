#pragma once

#include <algorithm>
#include <any>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ns_flags {
#pragma region helpers

/**
 * @brief throw 'std::runtime_error' exception from 'where', and message is 'msg'
 */
#define THROW_EXCEPTION(where, msg) \
  throw std::runtime_error(std::string("[ error from 'libflags'-'") + #where + "' ] " + msg)

/**
 * @brief when pass a empty 'std::vector<std::string>' to the 'fromStringVec'
 */
#define EMPTY_STRVEC_HANDLER(strVec) \
  if (strVec.empty()) {              \
    return this->_dargv;             \
  }

  template <typename ElemType>
  std::ostream &operator<<(std::ostream &os, const std::vector<ElemType> &vec) {
    os << '[';
    for (int i = 0; i < vec.size() - 1; ++i) {
      os << vec.at(i) << ", ";
    }
    if (!vec.empty()) {
      os << vec.back();
    }
    os << ']';
    return os;
  }
#pragma endregion

#pragma region arg types
  class Option;

  class MetaArg {
  public:
    friend class Option;

  public:
    /**
     * @brief get the default argument value
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @return ArgType::value_type
     */
    template <typename ArgType>
    typename ArgType::value_type getDefaultArgv() const {
      return std::any_cast<typename ArgType::value_type>(this->_dargv);
    }

    /**
     * @brief get the arguement value
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @return ArgType::value_type
     */
    template <typename ArgType>
    typename ArgType::value_type getArgv() const {
      return std::any_cast<typename ArgType::value_type>(this->_argv);
    }

    /**
     * @brief the type of the MetaArg
     *
     * @return std::string
     */
    virtual std::string type() const = 0;

    /**
     * @brief set the arguement's value from a std::vector<std::string>
     *
     * @param strVec the string vector
     * @return MetaArg&
     */
    MetaArg &setArgv(const std::vector<std::string> &strVec) {
      try {
        this->_argv = fromStringVec(strVec);
      } catch (...) {
        THROW_EXCEPTION(setupParser, "the argv(s) you passed for some option(s) is(are) invalid");
      }
      return *this;
    }

  protected:
    /**
     * @brief cast a string vector to a std::any object
     *
     * @param strVec the string vector
     * @return std::any
     */
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const = 0;

    MetaArg() = default;
    ~MetaArg() = default;

  protected:
    // arguement value
    std::any _argv;
    // default arguement value
    std::any _dargv;
  };

  class IntArg : public MetaArg {
  public:
    friend class Option;
    using value_type = int;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      return std::stoi(strVec.front());
    }
    virtual std::string type() const override {
      return "IntArg";
    }
  };

  class DoubleArg : public MetaArg {
  public:
    friend class Option;
    using value_type = double;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      return std::stod(strVec.front());
    }
    virtual std::string type() const override {
      return "DoubleArg";
    }
  };

  class BoolArg : public MetaArg {
  public:
    friend class Option;
    using value_type = bool;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      if (strVec.empty()) {
        return true;
      }
      std::string lower = strVec.front();
      for (int i = 0; i != lower.size(); ++i) {
        lower.at(i) = std::tolower(lower.at(i));
      }
      if (lower == "true" || lower == "on" || lower == "1" || lower == "yes" || lower == "y") {
        return true;
      } else {
        return false;
      }
    }
    virtual std::string type() const override {
      return "BoolArg";
    }
  };

  class StringArg : public MetaArg {
  public:
    friend class Option;
    using value_type = std::string;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      return strVec.front();
    }
    virtual std::string type() const override {
      return "StringArg";
    }
  };

  class IntVecArg : public MetaArg {
  public:
    friend class Option;
    using value_type = std::vector<int>;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      std::vector<int> iv;
      for (const auto &elem : strVec) {
        iv.push_back(std::stoi(elem));
      }
      return iv;
    }
    virtual std::string type() const override {
      return "IntVecArg";
    }
  };

  class DoubleVecArg : public MetaArg {
  public:
    friend class Option;
    using value_type = std::vector<double>;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      std::vector<int> dv;
      for (const auto &elem : strVec) {
        dv.push_back(std::stod(elem));
      }
      return dv;
    }
    virtual std::string type() const override {
      return "DoubleVecArg";
    }
  };

  class BoolVecArg : public MetaArg {
  public:
    friend class Option;
    using value_type = std::vector<bool>;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      std::vector<bool> bv;
      for (const auto &elem : strVec) {
        std::string lower = elem;
        for (int i = 0; i != lower.size(); ++i) {
          lower.at(i) = std::tolower(lower.at(i));
        }
        if (lower == "true" || lower == "on" || lower == "1" || lower == "yes" || lower == "y") {
          bv.push_back(true);
        } else {
          bv.push_back(false);
        }
      }
      return bv;
    }
    virtual std::string type() const override {
      return "BoolVecArg";
    }
  };

  class StringVecArg : public MetaArg {
  public:
    friend class Option;
    using value_type = std::vector<std::string>;

  protected:
    virtual std::any fromStringVec(const std::vector<std::string> &strVec) const override {
      EMPTY_STRVEC_HANDLER(strVec);
      return strVec;
    }
    virtual std::string type() const override {
      return "StringVecArg";
    }
  };

#pragma endregion

#pragma region options

  enum class OptionProp {
    /**
     * @brief options
     */
    OPTIONAL,
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

  class OptionParser;

  class Option {
  public:
    friend std::ostream &operator<<(std::ostream &os, const Option &op);
    friend class OptionParser;

  public:
    template <typename ArgType>
    std::string info() const {
      std::stringstream stream;
      stream << "{'opt': " << this->_opt << ", 'type': " << this->_arg->type()
             << ", 'desc': " << this->_desc << ", 'prop': " << this->_prop
             << ", 'default': " << this->_arg->getDefaultArgv<ArgType>()
             << ", 'value': " << this->_arg->getArgv<ArgType>() << "}";
      return stream.str();
    }

  protected:
    Option() = default;

    /**
     * @brief create an option
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param optName the option's name
     * @param defaultArgv the default arguement value
     * @param desc the describe of the arguement
     * @param prop the prop of option
     * @return Option
     */
    template <typename ArgType>
    static Option create(const std::string &optName, const typename ArgType::value_type &defaultArgv,
                         const std::string &desc, OptionProp prop = OptionProp::OPTIONAL) {
      Option op = Option();
      op._opt = optName;
      op._desc = desc;
      op._prop = prop;
      op._arg = std::make_shared<ArgType>();
      op._arg->_dargv = op._arg->_argv = defaultArgv;
      return op;
    }

  private:
    std::string _opt;
    std::shared_ptr<MetaArg> _arg;
    std::string _desc;
    OptionProp _prop;
  };

  /**
   * @brief overload the operator '<<' for 'Option'
   */
  std::ostream &operator<<(std::ostream &os, const Option &op) {
    os << "{'opt': " << op._opt << ", 'type': " << op._arg->type()
       << ", 'desc': " << op._desc << ", 'prop': " << op._prop << "}";
    return os;
  }

#pragma endregion

#pragma region parser

  /**
   * @brief the main class
   */
  class OptionParser {
  public:
    friend std::ostream &operator<<(std::ostream &os, const OptionParser &parser);

  public:
    OptionParser() {
      Option help = Option::create<StringArg>(this->HELP_OPTION_NAME, "", "display the help docs", OptionProp::OPTIONAL);
      Option version = Option::create<StringArg>(this->VERSION_OPTION_NAME, "", "display the version of this program", OptionProp::OPTIONAL);
      this->_options.insert({this->HELP_OPTION_NAME, help});
      this->_options.insert({this->VERSION_OPTION_NAME, version});
    }

  public:
    /**
     * @brief add an option to the option parser
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param optName the option's name
     * @param defaultArgv the default arguement value
     * @param desc the describe of the arguement
     * @param prop the prop of option
     * @return OptionParser&
     */
    template <typename ArgType>
    OptionParser &addOption(const std::string &optName,
                            const typename ArgType::value_type &defaultArgv,
                            const std::string &desc,
                            OptionProp prop = OptionProp::OPTIONAL) {
      if (this->_options.find(optName) != this->_options.end()) {
        THROW_EXCEPTION(addOption, "the option named '" + optName + "'has been registered");
      }
      this->_options.insert({optName, Option::create<ArgType>(optName, defaultArgv, desc, prop)});
      return *this;
    }

    /**
     * @brief add an option to the option parser
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param defaultArgv the default arguement value
     * @param desc the describe of the arguement
     * @param prop the prop of option
     * @return OptionParser&
     */
    template <typename ArgType>
    OptionParser &setDefaultOption(const typename ArgType::value_type &defaultArgv,
                                   const std::string &desc = "the default option",
                                   OptionProp prop = OptionProp::OPTIONAL) {
      Option defaultOption = Option::create<ArgType>(this->DEFAULT_OPTION_NAME, defaultArgv, desc, prop);
      this->_options.insert({this->DEFAULT_OPTION_NAME, defaultOption});
      return *this;
    }

    /**
     * @brief get the default arguement value of the option
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param optName the option's name
     * @return ArgType::value_type
     */
    template <typename ArgType>
    typename ArgType::value_type getOptionDefaultArgv(const std::string &optionName) const {
      if (auto iter = this->_options.find(optionName); iter == this->_options.end()) {
        THROW_EXCEPTION(getOptionDefaultArgv, "there isn't option named '" + optionName + "'");
      } else {
        return iter->second._arg->getDefaultArgv<ArgType>();
      }
    }

    /**
     * @brief get the default arguement value of the option
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param optName the option's name
     * @return ArgType::value_type
     */
    template <typename ArgType>
    typename ArgType::value_type getOptionArgv(const std::string &optionName) const {
      if (auto iter = this->_options.find(optionName); iter == this->_options.end()) {
        THROW_EXCEPTION(getOptionArgv, "there isn't option named '" + optionName + "'");
      } else {
        return iter->second._arg->getArgv<ArgType>();
      }
    }

    /**
     * @brief get the default arguement value of the option
     *
     * @tparam ArgType the arguement class type. eg: IntArg, DoubleArg
     * @param optName the option's name
     * @return ArgType::value_type
     */
    template <typename ArgType>
    typename ArgType::value_type getDefaultOptionArgv() const {
      if (auto iter = this->_options.find(this->DEFAULT_OPTION_NAME); iter == this->_options.end()) {
        THROW_EXCEPTION(getDefaultOptionArgv, "you haven't set the 'default-option'");
      } else {
        return iter->second._arg->getArgv<ArgType>();
      }
    }

    /**
     * @brief set up the parser
     *
     * @param argc the count of the arguement
     * @param argv the value of the arguement
     * @return OptionParser&
     */
    OptionParser &setupParser(int argc, char const *argv[]) {
      // first, generate the corresponding help document and version number according to the set options
      if (this->_options.at(this->VERSION_OPTION_NAME)._arg->getArgv<StringArg>().empty()) {
        this->autoGenerateVersion();
      }
      if (this->_options.at(this->HELP_OPTION_NAME)._arg->getArgv<StringArg>().empty()) {
        this->autoGenerateHelp(argv[0]);
      }

      // find all valid options in the parameter list and record their names and locations
      std::vector<std::pair<std::string, std::size_t>> optionInfo;
      std::unordered_set<std::string> optionsPassed;
      for (int i = 1; i != argc; ++i) {
        if (this->optionOrArgv(argv[i])) {
          // is an option
          auto optionName = std::string(argv[i]).substr(2);

          // is help or version options
          if (optionName == this->HELP_OPTION_NAME) {
            throw std::runtime_error(this->getOptionArgv<StringArg>(this->HELP_OPTION_NAME));
          } else if (optionName == this->VERSION_OPTION_NAME) {
            throw std::runtime_error(argv[0] + std::string(" version: ") +
                                     this->getOptionArgv<StringArg>(this->VERSION_OPTION_NAME));
          }

          optionInfo.push_back({optionName, i});
          optionsPassed.insert(optionName);
        }
      }

      // check whether any missing options have not been passed in according to the properties of the set options
      if (argc > 1) {
        if (optionInfo.empty() || optionInfo.front().second > 1) {
          optionsPassed.insert(this->DEFAULT_OPTION_NAME);
        }
      }
      for (const auto &[name, option] : this->_options) {
        if (option._prop == OptionProp::REQUIRED) {
          auto iter = optionsPassed.find(name);
          if (iter == optionsPassed.cend()) {
            THROW_EXCEPTION(setupParser, "the option named '--" + name + "' is 'OptionProp::REQUIRED', but you didn't use it");
          }
        }
      }

      // divide the parameter list according to the position of the option
      // and set it to the value of the corresponding option
      if (optionInfo.empty()) {
        return *this;
      }

      std::vector<std::string> strVec;
      for (int i = 1; i != optionInfo.front().second; ++i) {
        strVec.push_back(argv[i]);
      }
      if (auto iter = this->_options.find(this->DEFAULT_OPTION_NAME); iter != this->_options.end()) {
        this->_options.at(this->DEFAULT_OPTION_NAME)._arg->setArgv(strVec);
      }

      for (int i = 0; i < optionInfo.size() - 1; ++i) {
        strVec.clear();
        auto curOption = optionInfo.at(i);
        auto nextOption = optionInfo.at(i + 1);
        for (int j = curOption.second + 1; j < nextOption.second; ++j) {
          strVec.push_back(argv[j]);
        }
        this->_options.at(curOption.first)._arg->setArgv(strVec);
      }

      strVec.clear();
      for (int j = optionInfo.back().second + 1; j < argc; ++j) {
        strVec.push_back(argv[j]);
      }
      this->_options.at(optionInfo.back().first)._arg->setArgv(strVec);

      return *this;
    }

    /**
     * @brief set the version string
     *
     * @param version the string
     * @return OptionParser&
     */
    OptionParser &setVersion(const std::string &version) {
      this->_options.at(this->VERSION_OPTION_NAME)._arg->setArgv({version});
      return *this;
    }

    /**
     * @brief set the help string
     *
     * @param help the string
     * @return OptionParser&
     */
    OptionParser &setHelp(const std::string &help) {
      this->_options.at(this->HELP_OPTION_NAME)._arg->setArgv({help});
      return *this;
    }

    /**
     * @brief get the option's info
     *
     * @tparam ArgType the type of arguement
     * @param optionName the name of option
     * @return std::string
     */
    template <typename ArgType>
    std::string getOptionInfo(const std::string &optionName) const {
      if (auto iter = this->_options.find(optionName); iter == this->_options.end()) {
        THROW_EXCEPTION(getOptionArgv, "there isn't option named '" + optionName + "'");
      } else {
        return iter->second.info<ArgType>();
      }
    }

    /**
     * @brief get the default option's Info
     *
     * @tparam ArgType the type of arguement
     * @return std::string
     */
    template <typename ArgType>
    std::string getDefaultOptionInfo() const {
      if (auto iter = this->_options.find(this->DEFAULT_OPTION_NAME); iter == this->_options.end()) {
        THROW_EXCEPTION(getOptionArgv, "there isn't option named '" + this->DEFAULT_OPTION_NAME + "'");
      } else {
        return iter->second.info<ArgType>();
      }
    }

  protected:
    /**
     * @brief handle the param
     *
     * @param str the param
     * @return true is an option
     * @return false is an argv
     * @throw std::runtime_error invalid option
     */
    bool optionOrArgv(const std::string &str) const {
      if (str.size() < 3) {
        return false;
      } else {
        if (str.substr(0, 2) == "--") {
          auto optionName = str.substr(2);
          // is not an valid option
          if (auto iter = this->_options.find(optionName) == this->_options.end()) {
            THROW_EXCEPTION(setupParser, "the option named '--" + optionName + "' is invalid");
          } else {
            return true;
          }
        } else {
          return false;
        }
      }
    }

    /**
     * @brief automatically generate the help document of the program according to the set options
     *
     * @param programName the name of the program
     * @return OptionParser&
     */
    OptionParser &autoGenerateHelp(const std::string &programName) {
      auto defaulrOptionIter = this->_options.find(this->DEFAULT_OPTION_NAME);
      auto helpOptionIter = this->_options.find(this->HELP_OPTION_NAME);
      auto versionOptionIter = this->_options.find(this->VERSION_OPTION_NAME);
      std::stringstream stream;
      // the main usage of this program
      stream << "Usage: " << programName;
      if (defaulrOptionIter != this->_options.end()) {
        stream << " [def-opt target(s)]";
      }
      // the header of the help docs
      stream << " [--option target(s)] ...\n\n    "
             << std::setw(15) << std::left << "Options"
             << std::setw(15) << std::left << "Property"
             << std::setw(15) << std::left << "Type"
             << "Describes\n";
      stream << std::string(62, '-') << '\n';
      // display help docs for default-option
      if (defaulrOptionIter != this->_options.end()) {
        stream << "  --" << std::setw(15) << std::left << defaulrOptionIter->first
               << std::setw(15) << std::left << defaulrOptionIter->second._prop
               << std::setw(15) << std::left << defaulrOptionIter->second._arg->type()
               << defaulrOptionIter->second._desc << "\n\n";
      }
      // display help docs for options set by user
      for (const auto &[name, option] : this->_options) {
        if (name == this->VERSION_OPTION_NAME || name == this->HELP_OPTION_NAME || name == this->DEFAULT_OPTION_NAME) {
          continue;
        }
        stream << "  --" << std::setw(15) << std::left << name
               << std::setw(15) << std::left << option._prop
               << std::setw(15) << std::left << option._arg->type()
               << option._desc << '\n';
      }
      // display help docs for 'help' and 'version' options
      stream << "\n  --" << std::setw(15) << std::left << this->HELP_OPTION_NAME
             << std::setw(15) << std::left << helpOptionIter->second._prop
             << std::setw(15) << std::left << helpOptionIter->second._arg->type()
             << helpOptionIter->second._desc;
      stream << "\n  --" << std::setw(15) << std::left << this->VERSION_OPTION_NAME
             << std::setw(15) << std::left << versionOptionIter->second._prop
             << std::setw(15) << std::left << versionOptionIter->second._arg->type()
             << versionOptionIter->second._desc;
      // the tail of the help docs
      stream << "\n\nhelp docs for program \"" + programName + "\"";
      this->_options.at(this->HELP_OPTION_NAME)._arg->setArgv({stream.str()});
      return *this;
    }

    /**
     * @brief automatically generate the version of the program according to the set options
     *
     * @return OptionParser&
     */
    OptionParser &autoGenerateVersion() {
      this->_options.at(this->VERSION_OPTION_NAME)._arg->setArgv({"0.0.1"});
      return *this;
    }

  private:
    std::unordered_map<std::string, Option> _options;

    const std::string HELP_OPTION_NAME = "help";
    const std::string VERSION_OPTION_NAME = "version";
    const std::string DEFAULT_OPTION_NAME = "def-opt";
  };

  /**
   * @brief overload the operator '<<' for 'OptionParser'
   */
  std::ostream &operator<<(std::ostream &os, const OptionParser &parser) {
    os << "OptionParser Info: {\n";
    for (const auto &elem : parser._options) {
      os << "  " << elem.second << ";\n";
    }
    os << "}";
    return os;
  }
#undef THROW_EXCEPTION
#undef EMPTY_STRVEC_HANDLER

#pragma endregion
} // namespace ns_flags
