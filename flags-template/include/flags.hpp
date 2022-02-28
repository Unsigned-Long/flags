#pragma once

/**
 * @file flags.hpp
 * @author csl (3079625093@qq.com)
 * @version 0.1
 * @date 2022-02-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <any>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ns_flags {

#pragma region struct 'ArgType'

  struct ArgType {
  public:
    friend class ArgParser;

  public:
    /**
     * @brief options' type
     */
    using INT = int;
    using DOUBLE = double;
    using BOOL = bool;
    using STRING = std::string;
    using INT_VEC = std::vector<int>;
    using DOUBLE_VEC = std::vector<double>;
    using BOOL_VEC = std::vector<bool>;
    using STRING_VEC = std::vector<std::string>;

    friend class ArgParser;

    /**
     * @brief cast a std::any type object to a string
     * @attention it just supports the types in ArgType
     *
     * @param any the std::any type object
     * @return std::string
     */
    static std::string to_string(const std::any &any) {
      try {
        return std::to_string(any_cast<INT>(any));
      } catch (...) {
      }
      try {
        return std::to_string(any_cast<DOUBLE>(any));
      } catch (...) {
      }
      try {
        std::stringstream stream;
        stream << std::boolalpha << any_cast<BOOL>(any);
        return stream.str();
      } catch (...) {
      }
      try {
        return any_cast<STRING>(any);
      } catch (...) {
      }
      try {
        return format_vector(any_cast<INT_VEC>(any));
      } catch (...) {
      }
      try {
        return format_vector(any_cast<DOUBLE_VEC>(any));
      } catch (...) {
      }
      try {
        return format_vector(any_cast<BOOL_VEC>(any));
      } catch (...) {
      }
      try {
        return format_vector(any_cast<STRING_VEC>(any));
      } catch (...) {
      }
      throw std::runtime_error(
          "[ error from 'ArgType::to_string' ] can't cast type 'any' to type "
          "'std::string'");
      return "'to_string' failed";
    }

  protected:
#pragma region 'cast' and 'make' methods for 'std::any'
    /**
     * @brief cast the 'any' type to 'Type'
     *
     * @tparam Type the target type
     * @param any the std::any object, it containes a sheard pointer
     * @return Type&
     */
    template <typename Type>
    static Type &any_cast(const std::any &any) {
      try {
        return *(std::any_cast<std::shared_ptr<Type>>(any));
      } catch (const std::exception &e) {
        throw std::runtime_error(std::string("[ error from 'ArgType::any_cast' ] "
                                             "can't cast type 'any' to type '") +
                                 typeid(Type).name() + "'");
      }
    }

    /**
     * @brief make a std::any object
     *
     * @tparam Type the type of the arg_val
     * @param arg_val the value
     * @return std::any
     */
    template <typename Type>
    static std::any make_any(Type arg_val) {
      return std::any(std::make_shared<Type>(arg_val));
    }
#pragma endregion

#pragma region 'asign' methods for 'std::any'
    /**
     * @brief using a string vector to asign a std::any type object
     * @attention it just supports the types in ArgType
     *
     * @param any the std::any type object
     * @param str_vec the string vector
     * @return true if asign succeeded
     * @return false if asign failed
     */
    static bool any_asign(const std::any &any,
                          const std::vector<std::string> &str_vec) {
      try {
        auto &arg_val = ArgType::any_cast<ArgType::INT_VEC>(any);
        arg_val.clear();
        for (const auto &elem : str_vec)
          arg_val.push_back(std::stoi(elem));
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::DOUBLE_VEC>(any);
        arg_val.clear();
        for (const auto &elem : str_vec)
          arg_val.push_back(std::stod(elem));
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::BOOL_VEC>(any);
        arg_val.clear();
        for (const auto &elem : str_vec) {
          std::string temp_str = ArgType::tolower(elem);
          if (temp_str == "true" || temp_str == "on")
            arg_val.push_back(true);
          else if (temp_str == "false" || temp_str == "off")
            arg_val.push_back(false);
          else
            arg_val.push_back(std::stoi(elem));
        }
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::STRING_VEC>(any);
        arg_val.clear();
        for (const auto &elem : str_vec)
          arg_val.push_back(elem);
        return true;
      } catch (...) {
      }
      return false;
    }

    /**
     * @brief using a string to asign a std::any type object
     * @attention it just supports the types in ArgType
     *
     * @param any the std::any type object
     * @param str_val the string
     * @return true if asign succeeded
     * @return false if asign failed
     */
    static bool any_asign(const std::any &any, const std::string &str_val) {
      std::stringstream stream;
      try {
        auto &arg_val = ArgType::any_cast<ArgType::INT>(any);
        stream << str_val;
        stream >> arg_val;
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::DOUBLE>(any);
        stream << str_val;
        stream >> arg_val;
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::BOOL>(any);
        stream << str_val;
        stream >> arg_val;
        std::string temp_str = ArgType::tolower(str_val);
        if (temp_str == "true" || temp_str == "on")
          arg_val = true;
        else if (temp_str == "false" || temp_str == "off")
          arg_val = false;
        return true;
      } catch (...) {
      }
      try {
        auto &arg_val = ArgType::any_cast<ArgType::STRING>(any);
        stream << str_val;
        stream >> arg_val;
        return true;
      } catch (...) {
      }
      return false;
    }
#pragma endregion

#pragma region help methods

    /**
     * @brief translate a string to a lower string
     *
     * @param str the sourc string
     * @return std::string
     */
    static std::string tolower(const std::string &str) {
      std::string temp_str(str.size(), ' ');
      for (int i = 0; i != str.size(); ++i)
        temp_str.at(i) = std::tolower(str.at(i));
      return temp_str;
    }

    template <typename Type>
    static bool type_check() {
      if (std::is_same<Type, ArgType::INT>::value)
        return true;
      if (std::is_same<Type, ArgType::DOUBLE>::value)
        return true;
      if (std::is_same<Type, ArgType::BOOL>::value)
        return true;
      if (std::is_same<Type, ArgType::STRING>::value)
        return true;
      if (std::is_same<Type, ArgType::INT_VEC>::value)
        return true;
      if (std::is_same<Type, ArgType::DOUBLE_VEC>::value)
        return true;
      if (std::is_same<Type, ArgType::BOOL_VEC>::value)
        return true;
      if (std::is_same<Type, ArgType::STRING_VEC>::value)
        return true;
      return false;
    }

    /**
     * @brief output format for ordered container
     */
    template <typename ConType>
    static std::string format_vector(const ConType &c) {
      std::stringstream stream;
      stream << std::boolalpha << '[';
      if (c.empty()) {
        stream << "]";
        return stream.str();
      }
      auto iter = c.cbegin();
      for (; iter != (--c.cend()); ++iter)
        stream << *iter << ", ";
      stream << *iter << ']';
      return stream.str();
    }
#pragma endregion
  }; // namespace ns_flags

  /**
   * @brief override operator '<<' for type 'std::any'
   */
  static std::ostream &operator<<(std::ostream &os, const std::any &any) {
    os << ArgType::to_string(any);
    return os;
  }

#pragma endregion

#pragma region enum 'OptProp'
  enum class OptProp {
    /**
     * @brief options
     */
    OPTIONAL,
    REQUIRED
  };
  /**
   * @brief override operator '<<' for type 'OptProp'
   */
  static std::ostream &operator<<(std::ostream &os, const OptProp &obj) {
    switch (obj) {
    case OptProp::OPTIONAL:
      os << "optional";
      break;
    case OptProp::REQUIRED:
      os << "required";
      break;
    }
    return os;
  };

#pragma endregion

#pragma region struct 'OptInfo'

  struct OptInfo {
  private:
    /**
     * @brief the members
     */
    std::string _name;

    OptProp _prop;

    std::any _argv;
    std::any _argdv;

    std::string _desc;

  public:
    /**
     * @brief construct a new OptInfo object
     */
    OptInfo(const std::string &name, const std::any &argv, const std::any &argdv,
            const std::string &desc, OptProp prop = OptProp::OPTIONAL)
        : _name(name), _prop(prop), _argv(argv), _argdv(argdv), _desc(desc) {}

    OptInfo() = default;

    inline const std::string &name() const { return this->_name; }

    inline std::any &argv() { return this->_argv; }
    inline const std::any &argv() const { return this->_argv; }

    inline const std::any &argdv() const { return this->_argdv; }

    inline const std::string &desc() const { return this->_desc; }

    inline OptProp prop() const { return this->_prop; }
  };

  /**
   * @brief override operator '<<' for type 'OptInfo'
   */
  std::ostream &operator<<(std::ostream &os, const OptInfo &obj) {
    os << '{';
    os << "'name': " << obj.name() << ", 'prop': " << obj.prop()
       << ", 'argv': " << obj.argv() << ", 'defult': " << obj.argdv()
       << ", 'desc': " << obj.desc();
    os << '}';
    return os;
  }

#pragma endregion

#pragma region struct 'ExistOpt'
  // just for 'help' and 'version'
  struct ExistOpt {
  private:
    /**
     * @brief the members
     */
    OptInfo _opti;
    std::string _str;

  public:
    /**
     * @brief
     *
     */
    ExistOpt(const OptInfo &opti, const std::string &str)
        : _opti(opti), _str(str) {}

    inline OptInfo &opti() { return this->_opti; }
    inline const OptInfo &opti() const { return this->_opti; }

    inline std::string &str() { return this->_str; }
    inline const std::string &str() const { return this->_str; }
  };

#pragma endregion

#pragma region class 'ArgParser'
  class ArgParser {
  private:
    std::unordered_map<std::string, OptInfo> _opts;

    ExistOpt _help;
    ExistOpt _version;
    ExistOpt _nopt;

  public:
    /**
     * @brief the default and only constructor for ArgParser
     */
    ArgParser()
        : _help(OptInfo("help", ArgType::make_any<ArgType::STRING>(""),
                        ArgType::make_any<ArgType::STRING>("help docs"),
                        "get the help docs of this program", OptProp::OPTIONAL),
                ""),
          _version(OptInfo("version", ArgType::make_any<ArgType::STRING>(""),
                           ArgType::make_any<ArgType::STRING>("0.0.1"),
                           "get the version of this program", OptProp::OPTIONAL),
                   "0.0.1"),
          _nopt(
              OptInfo("no-opt", std::any(), std::any(),
                      "pass arguement(s) without any option", OptProp::OPTIONAL),
              "N") {}

#pragma region for 'no-opt'

    /**
     * @brief Set the 'no-opt'
     *
     * @tparam Type the type of 'no-opt'
     * @param default_value the default value of the 'no-opt'
     * @param prop the property of 'no-opt'
     * @param desc the describe of 'no-opt'
     */
    template <typename Type>
    inline void set_nopt(
        const Type &default_value, OptProp prop = OptProp::OPTIONAL,
        const std::string &desc = "pass arguement(s) without any option") {
      // check the 'type'
      if (!ArgType::type_check<Type>())
        throw std::runtime_error(
            "[ error from 'ArgParser::set_nopt' ] the tparam 'Type' you passed "
            "for '--no-opt' is invalid, please pass tparam according to types in "
            "'ArgType'");
      this->_nopt =
          ExistOpt(OptInfo("no-opt", ArgType::make_any<Type>(Type()),
                           ArgType::make_any<Type>(default_value), desc, prop),
                   "Y");
    }

    /**
     * @brief Get the value of 'no-opt' arguement(s)
     *
     * @tparam Type the type of 'no-opt'
     * @return const Type&
     */
    template <typename Type>
    inline const Type &get_noptv() const {
      // check the 'type'
      if (!ArgType::type_check<Type>())
        throw std::runtime_error(
            "[ error from 'ArgParser::get_noptv'] the tparam 'Type' "
            "you passed for '--no-opt' is invalid, "
            "please pass tparam according to types in 'ArgType'");
      return ArgType::any_cast<Type>(this->_nopt.opti().argv());
    }

    /**
     * @brief Get the info of 'no-opt'
     *
     * @return const OptInfo
     */
    inline const OptInfo get_nopti() const { return this->_nopt.opti(); }

#pragma endregion

#pragma region 'get' methods for 'opts'

    /**
     * @brief get the count of the options in the parser
     *
     * @return std::size_t
     */
    inline std::size_t get_optc() const { return this->_opts.size(); }

    /**
     * @brief Get the option's info named 'name'
     *
     * @param name the name of the option
     * @return const OptInfo&
     */
    inline const OptInfo &get_opti(const std::string &name) const {
      try {
        return this->_opts.at(name);
      } catch (const std::exception &e) {
        throw std::runtime_error(
            std::string("[ error from 'ArgParser::get_argi' ] there is not an "
                        "option named '--") +
            name + "'");
      }
    }

    /**
     * @brief Get the all options in the parser
     *
     * @return const auto&
     */
    inline const auto &get_opts() const { return this->_opts; }

    /**
     * @brief Get the value of an option according to name
     *
     * @tparam Type the type of this option's arguement(s)
     * @param name the name of this option
     * @return const Type&
     */
    template <typename Type>
    inline const Type &get_argv(const std::string &name) const {
      // check the 'type'
      if (!ArgType::type_check<Type>())
        throw std::runtime_error(
            "[ error from 'ArgParser::get_argv' ] the tparam 'Type' you passed "
            "for option '--" +
            name +
            "' is invalid, please pass tparam according to types in 'ArgType'");
      try {
        return ArgType::any_cast<Type>(this->_opts.at(name).argv());
      } catch (const std::exception &e) {
        throw std::runtime_error(
            std::string(
                "[ error from 'ArgParser::get_argv' ] can't get the 'value' of "
                "the option named '--") +
            name + "'");
      }
    }

    /**
     * @brief Get the default value of an option according to name
     *
     * @tparam Type the type of this option's arguement(s)
     * @param name the name of this option
     * @return const Type&
     */
    template <typename Type>
    inline const Type &get_argdv(const std::string &name) const {
      // check the 'type'
      if (!ArgType::type_check<Type>())
        throw std::runtime_error(
            "[ error from 'ArgParser::get_argdv' ] the tparam 'Type' you "
            "passed for option '--" +
            name +
            "' is invalid, please pass tparam according to types in 'ArgType'");
      try {
        return ArgType::any_cast<Type>(this->_opts.at(name).argdv());
      } catch (const std::exception &e) {
        throw std::runtime_error(
            std::string("[ error from 'ArgParser::get_argdv' ] can't get "
                        "the 'defalut-value' of the option named '--") +
            name + "'");
      }
    }

    /**
     * @brief Get the describe of the option named 'name'
     *
     * @param name the name of the option
     * @return const std::string&
     */
    inline const std::string &get_argdc(const std::string &name) const {
      try {
        return this->_opts.at(name).desc();
      } catch (const std::exception &e) {
        throw std::runtime_error(
            std::string(
                "[ error from 'ArgParser::get_argdc' ] can't get the describe of "
                "the option named '--") +
            name + "'");
      }
    }
#pragma endregion

#pragma region main methods

    /**
     * @brief add an option to the parser
     *
     * @tparam Type the type of the option's arguemrnt(s)
     * @param name the name of the option
     * @param defult_value the default value of the option's arguement(s)
     * @param desc the describe of the option
     * @param prop the property of this option
     */
    template <typename Type>
    void add_opt(const std::string &name, const Type &defult_value,
                 const std::string &desc, OptProp prop = OptProp::OPTIONAL) {
      // check the 'type'
      if (!ArgType::type_check<Type>())
        throw std::runtime_error(
            "[ error from 'ArgParser::add_opt' ] the tparam 'Type' you passed "
            "for option '--" +
            name +
            "' is invalid, please pass tparam according "
            "to types in 'ArgType'");
      auto arg_info = OptInfo(name, ArgType::make_any<Type>(Type()),
                              ArgType::make_any<Type>(defult_value), desc, prop);
      if (!this->_opts.insert({name, arg_info}).second) {
        auto error_info = std::string(
                              "[ error from 'ArgParser::add_opt' ] can't add "
                              "option named '--") +
                          name + "' again";
        throw std::runtime_error(error_info);
      }
    }

    /**
     * @brief Set the up the parser
     *
     * @param argc the count of the arguements
     * @param argv the values of the arguements
     */
    void setup_parser(int argc, char const *argv[]) {
      // gen the help docs for arguements
      this->gen_help(argv[0]);

      std::string cur_arg_name("");
      std::string last_arg_name("");
      std::vector<std::string> argv_vec;
      bool used_nopt = false;
      std::unordered_set<std::string> used_options;

      for (int i = 1; i != argc; ++i) {
        std::string temp_str(argv[i]);
        if (std::string_view(temp_str.c_str(), 2) == "--") {
          // if the argv is a param
          cur_arg_name = temp_str.substr(2);
          // the param is 'help' or 'version'
          if (cur_arg_name == "help") {
            throw std::runtime_error(this->_help.str());
          } else if (cur_arg_name == "version") {
            throw std::runtime_error(argv[0] + std::string(" version: ") +
                                     this->_version.str());
          }
          auto iter = this->_opts.find(cur_arg_name);
          if (iter == this->_opts.end()) {
            // the param is invalid
            throw std::runtime_error(
                std::string("some error(s) happened in the command line:\n") +
                "[ error from 'ArgParser::setup_parser' ] the option named '--" +
                cur_arg_name + "' is invalid, use '--help' option for help");
          } else {
            // cast the arguement
            if (!argv_vec.empty()) {
              if (last_arg_name.empty()) {
                // try to asign the no option arguement
                if (this->use_nopt()) {
                  if (!ArgType::any_asign(this->_nopt.opti().argv(),
                                          argv_vec.front())) {
                    if (!ArgType::any_asign(this->_nopt.opti().argv(),
                                            argv_vec)) {
                      throw std::runtime_error(
                          "[ error from 'ArgParser::setup_parser' ] unknow "
                          "error happened in 'ArgType::any-asign' when asign "
                          "the '--no-opt'");
                    }
                  }
                  used_nopt = true;
                }
              } else {
                // try to asign the option arguements
                auto arg_value = this->_opts.at(last_arg_name).argv();
                if (!ArgType::any_asign(arg_value, argv_vec.front())) {
                  if (!ArgType::any_asign(arg_value, argv_vec)) {
                    throw std::runtime_error(
                        "[ error from 'ArgParser::setup_parser' ] unknow werror "
                        "happened in 'ArgType::any-asign'");
                  }
                }
                used_options.insert(last_arg_name);
              }
              // clean the last argvVec
              argv_vec.clear();
            }
            last_arg_name = cur_arg_name;
          }
        } else {
          // the argv is not a param
          argv_vec.push_back(temp_str);
        }
      }
      if (!argv_vec.empty()) {
        if (last_arg_name.empty()) {
          // try to asign the no option arguement
          if (this->use_nopt()) {
            if (!ArgType::any_asign(this->_nopt.opti().argv(),
                                    argv_vec.front())) {
              if (!ArgType::any_asign(this->_nopt.opti().argv(), argv_vec)) {
                throw std::runtime_error(
                    "[ error from 'ArgParser::setup_parser' ] unknow werror "
                    "happened in 'ArgType::any-asign' when asign the '--no-opt'");
              }
            }
            used_nopt = true;
          }
        } else {
          // try to asign the option arguements
          auto arg_value = this->_opts.at(last_arg_name).argv();
          if (!ArgType::any_asign(arg_value, argv_vec.front())) {
            if (!ArgType::any_asign(arg_value, argv_vec)) {
              throw std::runtime_error(
                  "[ error from 'ArgParser::setup_parser' ] unknow werror "
                  "happened in 'ArgType::any-asign'");
            }
          }
          used_options.insert(last_arg_name);
        }
      }
      // check for the property of options
      if (prop_error(used_nopt, this->_nopt.opti().prop())) {
        throw std::runtime_error(
            "[ error from 'ArgParser::setup_parser' ] the property of the "
            "'--no-opt' is 'OptProp::required', but you didn't pass the "
            "arguement(s)");
      }
      // check the options' property
      for (const auto [name, info] : this->_opts) {
        if (prop_error(used_options.find(name) != used_options.end(),
                       info.prop())) {
          throw std::runtime_error(
              "[ error from 'ArgParser::setup_parser' ] the property of the "
              "option named '--" +
              name +
              "' is 'OptProp::required', but you didn't pass the arguement(s)");
        }
      }
      return;
    }
#pragma endregion

#pragma region 'set' methods for 'help' and 'version' options
    /**
     * @brief Set the help docs string for the parser
     *
     * @param str the help str to set
     */
    inline void set_help(const std::string &str) {
      this->_help.str() = str;
      return;
    }

    /**
     * @brief Set the version of the program
     *
     * @param str the version str
     */
    inline void set_version(const std::string &str) {
      this->_version.str() = str;
      return;
    }
#pragma endregion

  protected:
#pragma region help methods
    /**
     * @brief generate the help docs
     *
     * @param program_name the name of the program
     */
    void gen_help(const std::string &program_name) {
      // of the help str is not empty, then it means the user has set it
      if (!this->_help.str().empty()) {
        return;
      }
      std::stringstream stream;
      // the main usage of this program
      stream << "Usage: " << program_name;
      if (this->use_nopt()) {
        stream << " [no-opt]";
      }
      // the header of the help docs
      stream << " [--option target(s)] ...\n\n    " << std::setw(15) << std::left
             << "Options" << std::setw(15) << std::left << "property"
             << std::setw(20) << "Default Value"
             << "Describes\n"
             << std::string(67, '-') << '\n';
      // display help docs for no-opt
      if (this->use_nopt()) {
        stream << "  --" << std::setw(15) << std::left
               << this->_nopt.opti().name() << std::setw(15) << std::left
               << this->_nopt.opti().prop() << std::setw(20)
               << this->_nopt.opti().argdv() << this->_nopt.opti().desc()
               << "\n\n";
      }
      // display help docs for options set by user
      for (const auto &[name, info] : this->_opts)
        stream << "  --" << std::setw(15) << std::left << name << std::setw(15)
               << std::left << info.prop() << std::setw(20) << info.argdv()
               << info.desc() << '\n';
      // display help docs for 'help' and 'version' options
      stream << "\n  --" << std::setw(15) << std::left << "help" << std::setw(15)
             << std::left << this->_help.opti().prop() << std::setw(20)
             << this->_help.opti().argdv() << this->_help.opti().desc() << '\n';
      stream << "  --" << std::setw(15) << std::left << "version" << std::setw(15)
             << std::left << this->_version.opti().prop() << std::setw(20)
             << this->_version.opti().argdv() << this->_version.opti().desc()
             << '\n';
      // the tail of the help docs
      stream << "\nhelp docs for program \"" + program_name + "\"";
      this->_help.str() = stream.str();
      return;
    }

    /**
     * @brief judge whether the 'no-opt' is set
     * @attention 'Y' means it's set and 'N' means it's not set
     */
    inline bool use_nopt() { return this->_nopt.str().front() == 'Y'; }

    /**
     * @brief if the option not used but the property is OptProp::REQUIRED,
     * then return false
     *
     * @param used whether the option is used
     * @param prop the property of this option
     */
    inline bool prop_error(bool used, OptProp prop) {
      return !used && prop == OptProp::REQUIRED;
    }
#pragma endregion
  };

#pragma endregion

} // namespace ns_flags
