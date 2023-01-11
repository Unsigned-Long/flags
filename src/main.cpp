#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;

        const auto &age = parser.AddOption<Int>(
                "age", 18, "the age of the student", OptionProp::OPTIONAL,
                [](const Int::data_type &val) -> std::optional<std::string> {
                    if (val > 0) {
                        return {};
                    }
                    return "age must be greater than 0";
                }
        );
        const auto &odds = parser.AddOption<IntVec>(
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
        const auto &height = parser.AddOption<Float>(
                "height", 174.5f, "the height", OptionProp::OPTIONAL,
                [](const Float::data_type &val) -> std::optional<std::string> {
                    if (val > 0.0f) {
                        return {};
                    } else {
                        return "the value of height must be positive";
                    }
                }
        );
        const auto &name = parser.AddOption<String>(
                "name", "null", "the name string", OptionProp::REQUIRED,
                [](const String::data_type &str) -> std::optional<std::string> {
                    if (str.empty()) {
                        return "the name string cannot be an empty string";
                    }
                    return {};
                }
        );
        const auto &note = parser.AddDefaultOption<String>(
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
