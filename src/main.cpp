#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;
        std::shared_ptr<Argument> arg = std::make_shared<IntVec>();
        ns_priv::Option opt(
                "idx", "idxVec", Variable::Create<IntVec>({-1, 2, 3}), "the index of the data to compute",
                OptionProp::OPTIONAL, [](const std::shared_ptr<Argument> &obj) -> std::optional<std::string> {
                    for (const auto &item: obj->Boost<IntVec>()->GetData()) {
                        if (item < 0) { return "the elem should be greater or equal than 0"; }
                    }
                    return {};
                }
        );
        LOG_VAR(opt)
        opt.AssertOptionValue();

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
