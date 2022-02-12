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
        "[ error from lib-flags ] can't cast type 'any' to type 'std::string' "
        "in 'ArgType::to_string'");
    return "'to_string' failed";
  }

 protected:
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
      throw std::runtime_error(
          std::string(
              "[ error from lib-flags ] can't cast type 'any' to type '") +
          typeid(Type).name() + "' in 'ArgType::any_cast'" + ", exception: \"" +
          e.what() + '\"');
    }
  }

  /**
   * @brief make a std::any object
   *
   * @tparam Type the source type
   * @param arg_val the value
   * @return std::any
   */
  template <typename Type>
  static std::any make_any(Type arg_val) {
    return std::any(std::make_shared<Type>(arg_val));
  }

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
      for (const auto &elem : str_vec) arg_val.push_back(std::stoi(elem));
      return true;
    } catch (...) {
    }
    try {
      auto &arg_val = ArgType::any_cast<ArgType::DOUBLE_VEC>(any);
      arg_val.clear();
      for (const auto &elem : str_vec) arg_val.push_back(std::stod(elem));
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
      for (const auto &elem : str_vec) arg_val.push_back(elem);
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
    if (std::is_same<Type, ArgType::INT>::value) return true;
    if (std::is_same<Type, ArgType::DOUBLE>::value) return true;
    if (std::is_same<Type, ArgType::BOOL>::value) return true;
    if (std::is_same<Type, ArgType::STRING>::value) return true;
    if (std::is_same<Type, ArgType::INT_VEC>::value) return true;
    if (std::is_same<Type, ArgType::DOUBLE_VEC>::value) return true;
    if (std::is_same<Type, ArgType::BOOL_VEC>::value) return true;
    if (std::is_same<Type, ArgType::STRING_VEC>::value) return true;
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
    for (; iter != (--c.cend()); ++iter) stream << *iter << ", ";
    stream << *iter << ']';
    return stream.str();
  }
};  // namespace ns_flags

/**
 * @brief override operator '<<' for type 'std::any'
 */
static std::ostream &operator<<(std::ostream &os, const std::any &any) {
  os << ArgType::to_string(any);
  return os;
}

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

struct ArgInfo {
 private:
  /**
   * @brief the members
   */
  std::string _name;

  OptProp _prop;

  std::any _value;
  std::any _defult_value;

  std::string _desc;

 public:
  /**
   * @brief construct a new ArgInfo object
   */
  ArgInfo(const std::string &name, const std::any &value,
          const std::any &defult_value, const std::string &desc,
          OptProp prop = OptProp::OPTIONAL)
      : _name(name),
        _prop(prop),
        _value(value),
        _defult_value(defult_value),
        _desc(desc) {}

  ArgInfo() = default;

  inline const std::string &name() const { return this->_name; }

  inline std::any &value() { return this->_value; }
  inline const std::any &value() const { return this->_value; }

  inline const std::any &defult_value() const { return this->_defult_value; }

  inline const std::string &desc() const { return this->_desc; }

  inline OptProp prop() const { return this->_prop; }
};

/**
 * @brief override operator '<<' for type 'ArgInfo'
 */
std::ostream &operator<<(std::ostream &os, const ArgInfo &obj) {
  os << '{';
  os << "'name': " << obj.name() << ", 'prop': " << obj.prop()
     << ", 'value': " << obj.value() << ", 'defult': " << obj.defult_value()
     << ", 'desc': " << obj.desc();
  os << '}';
  return os;
}

// just for 'help' and 'version'
struct ExistArg {
 private:
  /**
   * @brief the members
   */
  ArgInfo _argi;
  std::string _str;

 public:
  /**
   * @brief
   *
   */
  ExistArg(const ArgInfo &argi, const std::string &str)
      : _argi(argi), _str(str) {}

  inline ArgInfo &argi() { return this->_argi; }
  inline const ArgInfo &argi() const { return this->_argi; }

  inline std::string &str() { return this->_str; }
  inline const std::string &str() const { return this->_str; }
};

class ArgParser {
 private:
  std::unordered_map<std::string, ArgInfo> _args;

  ExistArg _help;
  ExistArg _version;
  ExistArg _nopt_arg;

 public:
  /**
   * @brief the default and only constructor for ArgParser
   */
  ArgParser()
      : _help(ArgInfo("help", ArgType::make_any<ArgType::STRING>(""),
                      ArgType::make_any<ArgType::STRING>("help docs"),
                      "get the help docs of this program", OptProp::OPTIONAL),
              ""),
        _version(ArgInfo("version", ArgType::make_any<ArgType::STRING>(""),
                         ArgType::make_any<ArgType::STRING>("0.0.1"),
                         "get the version of this program", OptProp::OPTIONAL),
                 "0.0.1"),
        _nopt_arg(ArgInfo("nopt-arg(s)", std::any(), std::any(),
                          "arguement(s) without any option", OptProp::OPTIONAL),
                  "N") {}

  /**
   * @brief add a arguement to the parser
   *
   * @tparam Type the type of the arguement
   * @param name the name of the arguement
   * @param defult_value the default value of the arguement
   * @param desc the describe of the arguement
   * @param prop the property of this option
   */
  template <typename Type>
  void add_arg(const std::string &name, const Type &defult_value,
               const std::string &desc, OptProp prop = OptProp::OPTIONAL) {
    // check the 'type'
    if (!ArgType::type_check<Type>())
      throw std::runtime_error(
          "[ error from lib-flags ] the template param 'Type' you passed for "
          "option '--" +
          name +
          "' is invalid in 'ArgParser::add_arg', please pass 'Type' according "
          "to types in 'ArgType'");
    auto arg_info = ArgInfo(name, ArgType::make_any<Type>(Type()),
                            ArgType::make_any<Type>(defult_value), desc, prop);
    if (!this->_args.insert({name, arg_info}).second) {
      auto error_info =
          std::string("[ error from lib-flags ] can't add option named '--") +
          name + "' in 'ArgParser::add_arg' again";
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Set the nopt-arg(s) arguement
   *
   * @tparam Type the type of arguement
   * @param default_value the default value of the nopt-arg(s) arguement
   * @param prop the property of this option
   * @param desc
   */
  template <typename Type>
  void set_nopt_arg(
      const Type &default_value, OptProp prop = OptProp::OPTIONAL,
      const std::string &desc = "arguement(s) without any option") {
    // check the 'type'
    if (!ArgType::type_check<Type>())
      throw std::runtime_error(
          "[ error from lib-flags ] the template param 'Type' you passed for "
          "'--nopt-arg(s)' is invalid in 'ArgParser::set_nopt_argv', "
          "please pass 'Type' according to types in 'ArgType'");
    this->_nopt_arg =
        ExistArg(ArgInfo("nopt-arg(s)", ArgType::make_any<Type>(Type()),
                         ArgType::make_any<Type>(default_value), desc, prop),
                 "Y");
  }

  /**
   * @brief Get the nopt-arg(s) arguement's value
   *
   * @tparam Type the vaule type
   * @return const Type&
   */
  template <typename Type>
  inline const Type &get_nopt_argv() const {
    // check the 'type'
    if (!ArgType::type_check<Type>())
      throw std::runtime_error(
          "[ error from lib-flags ] the template param 'Type' you passed for "
          "'--nopt-arg(s)' is invalid in 'ArgParser::get_nopt_argv', "
          "please pass 'Type' according to types in 'ArgType'");
    return ArgType::any_cast<Type>(this->_nopt_arg.argi().value());
  }

  /**
   * @brief Get the nopt-arg(s) info object
   *
   * @return const ArgInfo
   */
  inline const ArgInfo get_nopt_argi() const { return this->_nopt_arg.argi(); }

  /**
   * @brief get the count of the arguements in the parser
   *
   * @return auto
   */
  inline std::size_t get_argc() const { return this->_args.size(); }

  /**
   * @brief Get the arg info object in the parser according to the name
   *
   * @param name the name of the arguement
   * @return const ArgInfo&
   */
  inline const ArgInfo &get_argi(const std::string &name) const {
    try {
      return this->_args.at(name);
    } catch (const std::exception &e) {
      auto error_info =
          std::string(
              "[ error from lib-flags ] can't get the 'arg-info' of "
              "the option named '--") +
          name + "' in 'ArgParser::get_argi', exception: \"" + e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Get the all arguements in the parser
   *
   * @return const auto&
   */
  inline const auto &get_args() const { return this->_args; }

  /**
   * @brief Get the value of an arguement according to name
   *
   * @tparam Type the type of this arguement
   * @param name the name of this arguement
   * @return Type&
   */
  template <typename Type>
  inline const Type &get_argv(const std::string &name) const {
    // check the 'type'
    if (!ArgType::type_check<Type>())
      throw std::runtime_error(
          "[ error from lib-flags ] the template param 'Type' you passed for "
          "option '--" +
          name +
          "' is invalid in 'ArgParser::get_argv', please pass 'Type' according "
          "to types in 'ArgType'");
    try {
      return ArgType::any_cast<Type>(this->_args.at(name).value());
    } catch (const std::exception &e) {
      auto error_info = std::string(
                            "[ error from lib-flags ] can't get the 'value' of "
                            "the option named '--") +
                        name + "' in 'ArgParser::get_argv', exception \"" +
                        e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Get the default value of an arguement according to name
   *
   * @tparam Type the type of this arguement
   * @param name the name of this arguement
   * @return const Type&
   */
  template <typename Type>
  inline const Type &get_argdv(const std::string &name) const {
    // check the 'type'
    if (!ArgType::type_check<Type>())
      throw std::runtime_error(
          "[ error from lib-flags ] the template param 'Type' you passed for "
          "option '--" +
          name +
          "' is invalid in 'ArgParser::get_argdv', please pass 'Type' "
          "according "
          "to types in 'ArgType'");
    try {
      return ArgType::any_cast<Type>(this->_args.at(name).defult_value());
    } catch (const std::exception &e) {
      auto error_info =
          std::string(
              "[ error from lib-flags ] can't get the 'defalut-value' of "
              "the option named '--") +
          name + "' in 'ArgParser::get_argdv', exception: \"" + e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Get the describe of the arguement named 'name'
   *
   * @param name
   * @return const std::string&
   */
  inline const std::string &get_argdc(const std::string &name) const {
    try {
      return this->_args.at(name).desc();
    } catch (const std::exception &e) {
      auto error_info = std::string(
                            "[ error from lib-flags ] can't get the 'desc' of "
                            "the option named '--") +
                        name + "' in 'ArgParser::get_argdc', exception: \"" +
                        e.what() + '\"';
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
                                   this->_help.str());
        }
        auto iter = this->_args.find(cur_arg_name);
        if (iter == this->_args.end()) {
          // the param is invalid
          throw std::runtime_error(
              std::string("some error(s) happened in the command line:\n") +
              "[ error from lib-flags ] the option named '--" + cur_arg_name +
              "' is invalid, use '--help' option for help.");
        } else {
          // cast the arguement
          if (!argv_vec.empty()) {
            if (last_arg_name.empty()) {
              // try to asign the no option arguement
              if (this->use_nopt_arg()) {
                if (!ArgType::any_asign(this->_nopt_arg.argi().value(),
                                        argv_vec.front())) {
                  if (!ArgType::any_asign(this->_nopt_arg.argi().value(),
                                          argv_vec)) {
                    throw std::runtime_error(
                        "[ error from lib-flags ] unknow werror happened in "
                        "'ArgType::any-asign' when asign the '--nopt-arg(s)'");
                  }
                }
                used_nopt = true;
              }
            } else {
              // try to asign the option arguements
              auto arg_value = this->_args.at(last_arg_name).value();
              if (!ArgType::any_asign(arg_value, argv_vec.front())) {
                if (!ArgType::any_asign(arg_value, argv_vec)) {
                  throw std::runtime_error(
                      "[ error from lib-flags ] unknow werror happened in "
                      "'ArgType::any-asign'");
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
        if (this->use_nopt_arg()) {
          if (!ArgType::any_asign(this->_nopt_arg.argi().value(),
                                  argv_vec.front())) {
            if (!ArgType::any_asign(this->_nopt_arg.argi().value(), argv_vec)) {
              throw std::runtime_error(
                  "[ error from lib-flags ] unknow werror happened in "
                  "'ArgType::any-asign' when asign the '--nopt-arg(s)'");
            }
          }
          used_nopt = true;
        }
      } else {
        // try to asign the option arguements
        auto arg_value = this->_args.at(last_arg_name).value();
        if (!ArgType::any_asign(arg_value, argv_vec.front())) {
          if (!ArgType::any_asign(arg_value, argv_vec)) {
            throw std::runtime_error(
                "[ error from lib-flags ] unknow werror happened in "
                "'ArgType::any-asign'");
          }
        }
        used_options.insert(last_arg_name);
      }
    }
    // check for the property of options
    if (!check_prop(used_nopt, this->_nopt_arg.argi().prop())) {
      throw std::runtime_error(
          "[ error from lib-flags ] the property of the '--nopt-arg(s)' "
          "is 'OptProp::required', but you didn't pass the arguement(s)");
    }
    for (const auto [name, info] : this->_args) {
      if (!check_prop(used_options.find(name) != used_options.end(),
                      info.prop())) {
        throw std::runtime_error(
            "[ error from lib-flags ] the property of the option named '--" +
            name +
            "' "
            "is 'OptProp::required', but you didn't pass the arguement(s)");
      }
    }
    return;
  }

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

 protected:
  /**
   * @brief generate the help docs
   *
   * @param program_name
   */
  void gen_help(const std::string &program_name) {
    if (!this->_help.str().empty()) {
      return;
    }
    std::stringstream stream;
    // the main usage of this program
    stream << "Usage: " << program_name;
    if (this->use_nopt_arg()) {
      stream << " [nopt-arg(s)]";
    }
    // the header of the help docs
    stream << " [--option target(s)] ...\n\n    " << std::setw(15) << std::left
           << "Options" << std::setw(15) << std::left << "property"
           << std::setw(20) << "Default Value"
           << "Describes\n"
           << std::string(67, '-') << '\n';
    // display help docs for nopt-arg(s)
    if (this->use_nopt_arg()) {
      stream << "  --" << std::setw(15) << std::left
             << this->_nopt_arg.argi().name() << std::setw(15) << std::left
             << this->_nopt_arg.argi().prop() << std::setw(20)
             << this->_nopt_arg.argi().defult_value()
             << this->_nopt_arg.argi().desc() << "\n\n";
    }
    // display help docs for options set by user
    for (const auto &[name, info] : this->_args)
      stream << "  --" << std::setw(15) << std::left << name << std::setw(15)
             << std::left << info.prop() << std::setw(20) << info.defult_value()
             << info.desc() << '\n';
    // display help docs for 'help' and 'version' options
    stream << "\n  --" << std::setw(15) << std::left << "help" << std::setw(15)
           << std::left << this->_help.argi().prop() << std::setw(20)
           << this->_help.argi().defult_value() << this->_help.argi().desc()
           << '\n';
    stream << "  --" << std::setw(15) << std::left << "version" << std::setw(15)
           << std::left << this->_version.argi().prop() << std::setw(20)
           << this->_version.argi().defult_value()
           << this->_version.argi().desc() << '\n';
    stream << "\nprogram help docs";
    this->_help.str() = stream.str();
    return;
  }

  inline bool use_nopt_arg() { return this->_nopt_arg.str().front() == 'Y'; }

  inline bool check_prop(bool used, OptProp prop) {
    return (used) || (!used && prop == OptProp::OPTIONAL);
  }
};
}  // namespace ns_flags
