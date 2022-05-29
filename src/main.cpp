#include "flags.hpp"

int main(int argc, char const *argv[]) {
  try {
    FLAGS_DEF_INT(age, age, "the age of the student", ns_flags_v2::OptionProp::OPTIONAL, 18);
    FLAGS_ASSERT_INT(age, "age must be greater than 0", [](int age) {
      return age > 0;
    });

    FLAGS_DEF_INT_VEC(odds, odds, "the odd number(s)", ns_flags_v2::OptionProp::OPTIONAL, 1, 3);
    FLAGS_ASSERT_INT_VEC(odds, "not all numbers entered are odd", [](const std::vector<int> &vec) {
      for (const auto &elem : vec) {
        if (elem % 2 == 0) {
          return false;
        }
      }
      return true;
    });

    FLAGS_DEF_FLOAT(height, height, "the height", ns_flags_v2::OptionProp::OPTIONAL, 174.5f);
    FLAGS_ASSERT_FLOAT(height, "the value of height must be positive", [](float val) {
      return val > 0.0f;
    });

    FLAGS_DEF_FLOAT_VEC(floatVals, fvs, "the float-type parameter(s)", ns_flags_v2::OptionProp::OPTIONAL, 3.14f, 2.71f, 1.414f);

    FLAGS_DEF_DOUBLE(weight, weight, "the weight", ns_flags_v2::OptionProp::OPTIONAL, 60.0);

    FLAGS_DEF_DOUBLE_VEC(doubleVals, dvs, "the double-type parameter(s)", ns_flags_v2::OptionProp::OPTIONAL, 1.1, 2.2);

    FLAGS_DEF_BOOL(sex, sex, "the sex(male[1], female[0])", ns_flags_v2::OptionProp::REQUIRED, true);

    FLAGS_DEF_BOOL_VEC(boolVals, bvs, "the bool-type parameter(s)", ns_flags_v2::OptionProp::REQUIRED, true, false);

    FLAGS_DEF_STRING(name, name, "the name", ns_flags_v2::OptionProp::REQUIRED);
    FLAGS_ASSERT_STRING(name, "the name string cannot be an empty string", [](const std::string &str) {
      return !str.empty();
    });

    FLAGS_DEF_STRING_VEC(likes, likes, "the likes", ns_flags_v2::OptionProp::OPTIONAL, "eat", "sleep");

    FLAGS_DEF_NO_OPTION(STRING, note, "a note", ns_flags_v2::OptionProp::OPTIONAL, "hello, world");

    std::cout << ns_flags_v2::ns_priv::parser << std::endl;

    ns_flags_v2::setupFlags(argc, argv);

    std::cout << ns_flags_v2::ns_priv::parser << std::endl;

  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
