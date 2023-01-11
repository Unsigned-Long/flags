#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        /**
         * FLAGS_DEF_INT(age, age, "the age of the student", ns_flags_v2::OptionProp::OPTIONAL, 18);
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

    // ...

    FLAGS_DEF_STRING(name, name, "the name", ns_flags_v2::OptionProp::REQUIRED);
    FLAGS_ASSERT_STRING(name, "the name string cannot be an empty string", [](const std::string &str) {
      return !str.empty();
    });

    // ...

    FLAGS_DEF_NO_OPTION(STRING, note, "a note", ns_flags_v2::OptionProp::OPTIONAL, "hello, world");

         */
        using namespace ns_flags;

        auto age = parser.AddOption<Int>(
                "age", 18, "the age of the student", OptionProp::OPTIONAL,
                [](const Int::data_type &val) -> std::optional<std::string> {
                    if (val > 0) {
                        return {};
                    }
                    return "age must be greater than 0";
                }
        );
        auto odds = parser.AddOption<IntVec>(
                "odds", {1, 3}, "the odd number(s)", OptionProp::OPTIONAL,
                [](const IntVec::data_type &vec) -> std::optional<std::string> {
                    for (const auto &item: vec) {
                        if (item % 2 == 0) {
                            return "not all numbers entered are odd";
                        }
                    }
                    return {};
                }
        );
        auto height = parser.AddOption<Float>(
                "height", 174.5f, "the height", OptionProp::OPTIONAL,
                [](const Float::data_type &val) -> std::optional<std::string> {
                    if (val > 0.0f) {
                        return {};
                    } else {
                        return "the value of height must be positive";
                    }
                }
        );
        auto note = parser.AddDefaultOption<String>(
                "hello, world!", "a note", OptionProp::OPTIONAL
        );
        parser.SetupFlags(argc, argv);
        LOG_VAR(age, odds, height, note)
        LOG_VAR(parser)
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
