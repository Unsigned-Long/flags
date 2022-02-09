#pragma once

#include <algorithm>
#include <any>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

/**
 * @brief the namespaces
 * @namespace ns_flags
 * @namespace ns_flags::ns_priv
 * @namespace ns_flags::ns_premain
 */

namespace ns_flags::ns_priv {

using param_cast_type = void (*)(const std::any&, const std::string&);

using vec_cast_type = void (*)(const std::any&,
                               const std::vector<std::string>&);

/**
 * @brief the string displayed in the help docs
 */
static std::string helpStr("");

/**
 * @brief whether generate the help docs automatically
 */
static bool autoGenHelpDocs = true;

/**
 * @brief the init string value of each param
 */
static std::unordered_map<std::string, std::vector<std::string>> parSetupMap;
#define INSERT_PARSETUPMAP(key, value) \
  ns_flags::ns_priv::parSetupMap.insert({key, value})

static std::unordered_map<std::string, param_cast_type> parCastMap;
#define INSERT_PARCASTMAP(key, value) \
  ns_flags::ns_priv::parCastMap.insert({key, value})

static std::unordered_map<std::string, vec_cast_type> vecCastMap;
#define INSERT_VECCASTMAP(key, value) \
  ns_flags::ns_priv::vecCastMap.insert({key, value})

static std::unordered_map<std::string, std::any> parFindMap;
#define INSERT_PARFINDMAP(key, value) \
  ns_flags::ns_priv::parFindMap.insert({key, value})

/**
 * @brief the describe of each param
 */
static std::unordered_map<std::string, std::string> parDescMap;
#define INSERT_PARDESCMAP(key, value) \
  ns_flags::ns_priv::parDescMap.insert({key, value})

}  // namespace ns_flags::ns_priv

namespace ns_flags {

/**
 * @brief support param types
 * [int, bool, std::string, double]
 * std::vectror<[int, bool, std::string, double]>
 */

/**
 * @brief define an 'int-type' param
 *
 * @param parName the name of the param
 * @param desc the describe of this param, used in the help docs
 * @param default the default value of the param
 */
#define DEFINE_INT(parName, desc, default) DEFINE(int, parName, desc, default)

/**
 * @brief define a 'std::vector<int>-type' param
 *
 * @param vecName the name of the vector
 * @param desc the describe of this vector, used in the help docs
 * @param __VA_ARGS__ the default elems of the vector
 */
#define DEFINE_INT_VEC(vecName, desc, ...) \
  DEFINE_VEC(std::vector<int>, vecName, desc, __VA_ARGS__)

/**
 * @brief define a 'bool-type' param
 *
 * @param parName the name of the param
 * @param desc the describe of this param, used in the help docs
 * @param default the default value of the param
 */
#define DEFINE_BOOL(parName, desc, default) DEFINE(bool, parName, desc, default)

/**
 * @brief define a 'std::vector<bool>-type' param
 *
 * @param vecName the name of the vector
 * @param desc the describe of this vector, used in the help docs
 * @param __VA_ARGS__ the default elems of the vector
 */
#define DEFINE_BOOL_VEC(vecName, desc, ...) \
  DEFINE_VEC(std::vector<bool>, vecName, desc, __VA_ARGS__)

/**
 * @brief define a 'std::string-type' param
 *
 * @param parName the name of the param
 * @param desc the describe of this param, used in the help docs
 * @param default the default value of the param
 */
#define DEFINE_STRING(parName, desc, default) \
  DEFINE(std::string, parName, desc, default)
/**
 * @brief define a 'std::vector<std::string>-type' param
 *
 * @param vecName the name of the vector
 * @param desc the describe of this vector, used in the help docs
 * @param __VA_ARGS__ the default elems of the vector
 */
#define DEFINE_STRING_VEC(vecName, desc, ...) \
  DEFINE_VEC(std::vector<std::string>, vecName, desc, __VA_ARGS__)

/**
 * @brief define a 'double-type' param
 *
 * @param parName the name of the param
 * @param desc the describe of this param, used in the help docs
 * @param default the default value of the param
 */
#define DEFINE_DOUBLE(parName, desc, default) \
  DEFINE(double, parName, desc, default)
/**
 * @brief define a 'std::vector<double>-type' param
 *
 * @param vecName the name of the vector
 * @param desc the describe of this vector, used in the help docs
 * @param __VA_ARGS__ the default elems of the vector
 */
#define DEFINE_DOUBLE_VEC(vecName, desc, ...) \
  DEFINE_VEC(std::vector<double>, vecName, desc, __VA_ARGS__)

/**
 * @brief define a param
 * [int, bool, std::string, double]
 * std::vectror<[int, bool, std::string, double]>
 */
#define DEFINE(parType, parName, desc, default)                                \
  namespace ns_flags {                                                         \
  static parType parName = parType(default);                                   \
  }                                                                            \
  PRE_MAIN(insert_##parName, INSERT_PARCASTMAP(#parName, PARAM_CAST(parType))) \
  PRE_MAIN(find_##parName,                                                     \
           INSERT_PARFINDMAP(#parName, std::any(&ns_flags::parName)))          \
  PRE_MAIN(desc_##parName, INSERT_PARDESCMAP(#parName, desc))

#define DEFINE_VEC(vecType, vecName, desc, ...)                       \
  namespace ns_flags {                                                \
  static vecType vecName = vecType{__VA_ARGS__};                      \
  }                                                                   \
  PRE_MAIN(insert_##vecName,                                          \
           INSERT_VECCASTMAP(#vecName, PARAM_CAST_VEC(vecType)))      \
  PRE_MAIN(find_##vecName,                                            \
           INSERT_PARFINDMAP(#vecName, std::any(&ns_flags::vecName))) \
  PRE_MAIN(desc_##vecName, INSERT_PARDESCMAP(#vecName, desc))

/**
 * @brief execute statement before main function
 *
 * @param name the struct type name
 * @param __VA_ARGS__ the statement to execute
 */
#define PRE_MAIN(name, ...)        \
  namespace ns_flags::ns_premain { \
  struct Pre_##name {              \
    Pre_##name() { __VA_ARGS__; }  \
  } pre_##name;                    \
  }

/**
 * @brief generate the function that casts the argv from 'string' to 'tarType'
 */
#define PARAM_CAST(tarType)                                      \
  [](const std::any& param, const std::string& strVal) -> void { \
    std::stringstream castStream;                                \
    castStream << strVal;                                        \
    castStream >> *(std::any_cast<tarType*>(param));             \
    return;                                                      \
  }
/**
 * @brief generate the function that casts the argv[vector] from 'string' to
 * 'tarType'
 */
#define PARAM_CAST_VEC(vecType)                                               \
  [](const std::any& param, const std::vector<std::string>& strVec) -> void { \
    std::stringstream castStream;                                             \
    vecType::value_type temp;                                                 \
    (std::any_cast<vecType*>(param))->clear();                                \
    for (const auto& elem : strVec) {                                         \
      castStream << elem;                                                     \
      castStream >> temp;                                                     \
      (std::any_cast<vecType*>(param))->push_back(temp);                      \
      castStream.clear();                                                     \
    }                                                                         \
    return;                                                                   \
  }

}  // namespace ns_flags

/**
 * @brief default params
 * [version, help]
 */
DEFINE_STRING(version, "the version of this program [default: 1.0]", "1.0")

DEFINE_BOOL(help, "display the help docs [default: 0]", false)

namespace ns_flags {
/**
 * @brief Set the string of the help docs
 * @attention if this method called, then the help docs will not generate
 * automatically
 * @param helpStr the help string
 */
static void setup_help(const std::string& helpStr) {
  ns_flags::ns_priv::autoGenHelpDocs = false;
  ns_flags::ns_priv::helpStr = helpStr;
  return;
}

/**
 * @brief Set the version of this program
 *
 * @param version the version string [default: 1.0]
 */
static void setup_version(const std::string& version) {
  ns_flags::version = version;
  return;
}

/**
 * @brief Get the param setup string value at the begining of the main function
 */
static auto get_param_setup_value() { return ns_flags::ns_priv::parSetupMap; }

/**
 * @brief Get the param describe that user defined
 */
static auto get_param_desc() { return ns_flags::ns_priv::parDescMap; }

/**
 * @brief set up the flags according the params user defined
 *
 * @param argc the count of the arguments
 * @param argv the value of the arguments
 */
static void setup_flags(int argc, char const* argv[]) {
  // if user set the 'autoGenHelpDocs' true, then gengerate the help docs
  // automatically
  if (ns_priv::autoGenHelpDocs) {
    ns_priv::helpStr = "Usage: " + std::string(argv[0]) +
                       " [options] [target] ...\n\nOptions:\n";
    std::stringstream stream;
    for (const auto& [param, desc] : ns_priv::parDescMap)
      stream << "  --" << std::setw(20) << std::left << param << desc << '\n';
    ns_flags::ns_priv::helpStr += stream.str();
  }

  std::string curParam("");
  std::string lastParam("");
  std::vector<std::string> argvVec;

  for (int i = 1; i != argc; ++i) {
    std::string tempStr(argv[i]);
    if (std::string_view(tempStr.c_str(), 2) == "--") {
      // if the argv is a param
      curParam = tempStr.substr(2);
      auto iter = ns_priv::parFindMap.find(curParam);
      if (iter == ns_priv::parFindMap.end()) {
        // the param is invalid
        throw std::runtime_error(
            std::string("some error(s) happened in the command line:\n") +
            "[ error ] the option named '--" + curParam +
            "' is invalid, use '--help' option for help.");
      } else {
        // the param is vaild
        if (curParam == "help") {
          throw std::runtime_error(ns_priv::helpStr + "\nProgram Help Docs");
        } else if (curParam == "version") {
          throw std::runtime_error(std::string(argv[0]) +
                                   " version: " + ns_flags::version);
        }
        // cast the argvVec
        if (!argvVec.empty()) {
          // for the first time, the argvVec is empty
          auto paramAny = ns_priv::parFindMap.find(lastParam)->second;
          if (auto castIter = ns_priv::parCastMap.find(lastParam);
              castIter != ns_priv::parCastMap.end()) {
            // is a single param [not a vector param]
            castIter->second(paramAny, argvVec.front());
          } else if (auto castIter = ns_priv::vecCastMap.find(lastParam);
                     castIter != ns_priv::vecCastMap.end()) {
            // is a vector param
            castIter->second(paramAny, argvVec);
          } else {
            throw std::runtime_error("unknow error happend");
          }
          ns_priv::parSetupMap.insert({lastParam, argvVec});
          // clean the last argvVec
          argvVec.clear();
        }
        lastParam = curParam;
      }
    } else {
      // the argv is not a param
      argvVec.push_back(tempStr);
    }
  }
  if (!argvVec.empty() && !lastParam.empty()) {
    // for the first time, the argvVec is empty
    auto paramAny = ns_priv::parFindMap.find(lastParam)->second;
    if (auto castIter = ns_priv::parCastMap.find(lastParam);
        castIter != ns_priv::parCastMap.end()) {
      // is a single param [not a vector param]
      castIter->second(paramAny, argvVec.front());
    } else if (auto castIter = ns_priv::vecCastMap.find(lastParam);
               castIter != ns_priv::vecCastMap.end()) {
      // is a vector param
      castIter->second(paramAny, argvVec);
    } else {
      throw std::runtime_error("unknow error happend");
    }
    ns_priv::parSetupMap.insert({lastParam, argvVec});
    // clean the last argvVec
    argvVec.clear();
  }
  return;
}
}  // namespace ns_flags
