#pragma once

#include <any>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ns_flags {

struct ArgType {
 public:
  /**
   * @brief options
   */
  using INT = int;
  using DOUBLE = double;
  using BOOL = bool;
  using STRING = std::string;
  using INT_VEC = std::vector<int>;
  using DOUBLE_VEC = std::vector<double>;
  using BOOL_VEC = std::vector<bool>;
  using STRING_VEC = std::vector<std::string>;

 public:
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
      auto error_info =
          std::string(
              "[ error from lib-flags ] can't cast type 'any' to type '") +
          typeid(Type).name() + "' in 'ArgType::any_cast'" + ", exception: \"" +
          e.what() + '\"';
      throw std::runtime_error(error_info);
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
   * @brief cast a std::any type object to a string
   * @attention it just supports the types in ArgType
   *
   * @param any the std::any type object
   * @return std::string
   */
  static std::string to_string(const std::any &any) {
#define TRY_RETURN(cast_type)                                 \
  try {                                                       \
    return std::to_string(ArgType::any_cast<cast_type>(any)); \
  } catch (...) {                                             \
  }
#define TRY_RETURN_VEC(cast_type)                                     \
  try {                                                               \
    return ArgType::format_vector(ArgType::any_cast<cast_type>(any)); \
  } catch (...) {                                                     \
  }

    try {
      std::stringstream stream;
      stream << std::boolalpha << ArgType::any_cast<ArgType::BOOL>(any);
      return stream.str();
    } catch (...) {
    }
    try {
      return ArgType::any_cast<ArgType::STRING>(any);
    } catch (...) {
    }
    TRY_RETURN(ArgType::INT);
    TRY_RETURN(ArgType::DOUBLE);
    TRY_RETURN_VEC(ArgType::INT_VEC);
    TRY_RETURN_VEC(ArgType::DOUBLE_VEC);
    TRY_RETURN_VEC(ArgType::BOOL_VEC);
    TRY_RETURN_VEC(ArgType::STRING_VEC);

    throw std::runtime_error(
        "[ error from lib-flags ] can't cast type 'any' to type 'std::string' "
        "in 'ArgType::to_string'");
    return "'to_string' failed";
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
      auto &arg_val = ArgType::any_cast<ArgType::INT_VEC>(any);
      arg_val.clear();
      for (const auto &elem : str_vec) arg_val.push_back(std::stoi(elem));
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
    try {
      auto &arg_val = ArgType::any_cast<ArgType::DOUBLE_VEC>(any);
      arg_val.clear();
      for (const auto &elem : str_vec) arg_val.push_back(std::stod(elem));
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

#define TRY_ASIGN(asign_type)                           \
  try {                                                 \
    auto &arg_val = ArgType::any_cast<asign_type>(any); \
    stream << str_val;                                  \
    stream >> arg_val;                                  \
    return true;                                        \
  } catch (...) {                                       \
  }

    TRY_ASIGN(ArgType::INT);
    TRY_ASIGN(ArgType::DOUBLE);
    TRY_ASIGN(ArgType::STRING);
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

    return false;
  }

 protected:
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

  /**
   * @brief output format for ordered container
   */
  template <typename ConType>
  static std::string format_vector(const ConType &c) {
    std::stringstream stream;
    stream << std::boolalpha << '[';
    if (c.empty()) {
      stream << "(empty)]";
      return stream.str();
    }
    auto iter = c.cbegin();
    for (; iter != (--c.cend()); ++iter) stream << *iter << ", ";
    stream << *iter << ']';
    return stream.str();
  }
};  // namespace ns_flags

static std::ostream &operator<<(std::ostream &os, const std::any &any) {
  os << ArgType::to_string(any);
  return os;
}

struct ArgInfo {
 private:
  /**
   * @brief the members
   */
  std::string _name;
  std::any _value;
  std::any _defult_value;
  std::string _desc;

 public:
  /**
   * @brief construct a new ArgInfo object
   */
  ArgInfo(const std::string &name, const std::any &value,
          const std::any &defult_value, const std::string &desc)
      : _name(name), _value(value), _defult_value(defult_value), _desc(desc) {}

  ArgInfo() = delete;

  inline const std::string &name() const { return this->_name; }

  inline std::any &value() { return this->_value; }
  inline const std::any &value() const { return this->_value; }

  inline const std::any &defult_value() const { return this->_defult_value; }

  inline const std::string &desc() const { return this->_desc; }
};
/**
 * @brief override operator '<<' for type 'ArgInfo'
 */
std::ostream &operator<<(std::ostream &os, const ArgInfo &obj) {
  os << '{';
  os << "'name': " << obj.name() << ", 'value': " << obj.value()
     << ", 'defult': " << obj.defult_value() << ", 'desc': " << obj.desc();
  os << '}';
  return os;
}

class ArgParser {
 private:
  std::unordered_map<std::string, ArgInfo> _args;
  bool _auto_gen_help;
  std::string _help_str;
  std::string _version_str;

 public:
  /**
   * @brief the default and only constructor for ArgParser
   */
  ArgParser() : _auto_gen_help(true), _help_str(""), _version_str("1.0") {
    this->add_arg<ArgType::STRING>("version", "1.0",
                                   "the version of this program");
    this->add_arg<ArgType::BOOL>("help", false,
                                 "get help docs of this program");
  }

  /**
   * @brief add a arguement to the parser
   *
   * @tparam Type the type of the arguement
   * @param name the name of the arguement
   * @param defult_value the default value of the arguement
   * @param desc the describe of the arguement
   */
  template <typename Type>
  void add_arg(const std::string &name, const Type &defult_value,
               const std::string &desc) {
    auto arg_info = ArgInfo(name, ArgType::make_any<Type>(Type()),
                            ArgType::make_any<Type>(defult_value), desc);
    if (!this->_args.insert({name, arg_info}).second) {
      auto error_info =
          std::string("[ error from lib-flags ] can't add arguement named '") +
          name + "' again";
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief get the count of the arguements in the parser
   *
   * @return auto
   */
  auto get_argc() const { return this->_args.size(); }

  /**
   * @brief Get the arg info object in the parser according to the name
   *
   * @param name the name of the arguement
   * @return const ArgInfo&
   */
  inline const ArgInfo &get_arg_info(const std::string &name) const {
    try {
      return this->_args.at(name);
    } catch (const std::exception &e) {
      auto error_info =
          std::string(
              "[ error from lib-flags ] can't get the 'arg-info' of "
              "the arguement named '") +
          name + "', exception: \"" + e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Get the all arguements in the parser
   *
   * @return const auto&
   */
  const auto &get_all_args() const { return this->_args; }

  /**
   * @brief Set the up the parser
   *
   * @param argc the count of the arguements
   * @param argv the values of the arguements
   */
  void setup_parser(int argc, char const *argv[]) {
    // gen the help docs for arguements
    this->gen_args_help(argv[0]);

    std::string cur_arg_name("");
    std::string last_arg_name("");
    std::vector<std::string> argv_vec;

    for (int i = 1; i != argc; ++i) {
      std::string temp_str(argv[i]);
      if (std::string_view(temp_str.c_str(), 2) == "--") {
        // if the argv is a param
        cur_arg_name = temp_str.substr(2);
        auto iter = this->_args.find(cur_arg_name);
        if (iter == this->_args.end()) {
          // the param is invalid
          throw std::runtime_error(
              std::string("some error(s) happened in the command line:\n") +
              "[ error from lib-flags ] the option named '--" + cur_arg_name +
              "' is invalid, use '--help' option for help.");
        } else {
          // the param is vaild
          if (cur_arg_name == "help") {
            std::cout << this->_help_str << '\n';
            std::exit(0);
          } else if (cur_arg_name == "version") {
            std::cout << argv[0] << " version: " << this->_version_str << '\n';
            std::exit(0);
          }
          // cast the arguement
          if (!argv_vec.empty()) {
            // for the first time, the argvVec is empty
            auto arg_value = this->get_arg_info(last_arg_name).value();
            if (!ArgType::any_asign(arg_value, argv_vec.front())) {
              if (!ArgType::any_asign(arg_value, argv_vec)) {
                throw std::runtime_error(
                    "[ error from lib-flags ] unknow werror happened in "
                    "'ArgType::any-asign'");
              }
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
    if (!argv_vec.empty() && !last_arg_name.empty()) {
      // for the first time, the argvVec is empty
      auto arg_value = this->get_arg_info(last_arg_name).value();
      if (!ArgType::any_asign(arg_value, argv_vec.front())) {
        if (!ArgType::any_asign(arg_value, argv_vec)) {
          throw std::runtime_error(
              "[ error from lib-flags ] unknow werror happened in "
              "'ArgType::any-asign'");
        }
      }
    }
    return;
  }

  /**
   * @brief Set the help docs string for the parser
   *
   * @param str the help str to set
   */
  void set_help(const std::string &str) {
    this->_help_str = str;
    this->_auto_gen_help = false;
    return;
  }

  /**
   * @brief Set the version of the program
   *
   * @param str the version str
   */
  void set_version(const std::string &str) {
    this->_version_str = str;
    return;
  }

  /**
   * @brief Get the value of an arguement according to name
   *
   * @tparam Type the type of this arguement
   * @param name the name of this arguement
   * @return Type&
   */
  template <typename Type>
  inline Type &get_arg_value(const std::string &name) {
    try {
      return ArgType::any_cast<Type>(this->_args.at(name).value());
    } catch (const std::exception &e) {
      auto error_info = std::string(
                            "[ error from lib-flags ] can't get the 'value' of "
                            "the arguement named '") +
                        name + "', exception \"" + e.what() + '\"';
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
  inline const Type &get_arg_default_value(const std::string &name) {
    try {
      return ArgType::any_cast<Type>(this->_args.at(name).defult_value());
    } catch (const std::exception &e) {
      auto error_info =
          std::string(
              "[ error from lib-flags ] can't get the 'defalut-value' of "
              "the arguement named '") +
          name + "', exception: \"" + e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

  /**
   * @brief Get the describe of the arguement named 'name' 
   *
   * @param name
   * @return const std::string&
   */
  inline const std::string &get_arg_desc(const std::string &name) const {
    try {
      return this->_args.at(name).desc();
    } catch (const std::exception &e) {
      auto error_info = std::string(
                            "[ error from lib-flags ] can't get the 'desc' of "
                            "the arguement named '") +
                        name + "', exception: \"" + e.what() + '\"';
      throw std::runtime_error(error_info);
    }
  }

 protected:
  /**
   * @brief generate the help docs
   *
   * @param program_name
   */
  inline void gen_args_help(const std::string &program_name) {
    if (!this->_auto_gen_help) {
      return;
    }
    std::stringstream stream;
    stream << "Usage: " << program_name << " [options] [target] ...\n\n    "
           << std::setw(15) << std::left << "Options" << std::setw(20)
           << "Default Value"
           << "Describes\n"
           << std::string(52, '-') << '\n';

    for (const auto &[name, info] : this->_args)
      stream << "  --" << std::setw(15) << std::left << name << std::setw(20)
             << info.defult_value() << info.desc() << '\n';
    stream << "\nprogram help docs";
    this->_help_str = stream.str();
    return;
  }
};
}  // namespace ns_flags
