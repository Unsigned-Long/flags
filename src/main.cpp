#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;
        std::shared_ptr<Argument> arg = std::make_shared<IntVec>();
        auto age = parser.AddOption<Int>("age", 12, "the age of the actor", OptionProp::OPTIONAL);
        auto idxVec = parser.AddOption<IntVec>("idx", {1, 2, 3}, "the index vector", OptionProp::OPTIONAL);
        auto sex = parser.AddDefaultOption<Bool>(true, "the sex", OptionProp::REQUIRED);
        parser.SetupFlags(argc, argv);
        LOG_VAR(parser)
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
