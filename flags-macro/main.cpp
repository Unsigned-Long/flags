#include "flags.h"
#define FORMAT_VECTOR
#define FORMAT_UNORDERED_MAP
#include "artwork/logger/logger.h"

/**
 * @brief define params
 */
DEFINE_STRING(name, "the name of the person", "");
DEFINE_DOUBLE(age, "the age of the person", 0.0);
DEFINE_BOOL(sex, "the sex of the person[female: 0, male:1]", 1);
DEFINE_STRING_VEC(likes, "the likes of the person", "");

int main(int argc, char const *argv[]) {
  /**
   * @brief you should use 'try-catch' struct
   */
  try {
    // set the version
    ns_flags::setup_version("2.0");
    // setup the flags
    ns_flags::setup_flags(argc, argv);

    // output the values
    INFO("here is the information of a person:");
    TEXT("name: ", ns_flags::name);
    TEXT("age: ", ns_flags::age);
    TEXT("sex: ", ns_flags::sex);
    TEXT("likes: ", ns_flags::likes, '\n');

    // output the setup string values
    INFO("the init param setup values:");
    ns_log::setFirSedName("option", "initVal");
    TEXT(ns_flags::get_param_setup_value(), '\n');

    // output the describes of the params
    INFO("the describes of params:");
    ns_log::setFirSedName("option", "desc");
    ns_log::setSplitor(",\n");
    TEXT(ns_flags::get_param_desc());

  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
