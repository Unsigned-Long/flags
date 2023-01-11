#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;
        std::shared_ptr<Argument> arg = std::make_shared<IntVec>();
        ns_priv::Option opt(
                "idx", "idxVec", Variable::Create<IntVec>({1, 2, 3}), "the index of the data to compute",
                OptionProp::OPTIONAL, [](const std::shared_ptr<Argument> &obj) -> std::optional<std::string> {
                    if (dynamic_cast<IntVec *>(obj.get())->GetData().front() > 12) {
                        return "";
                    } else {
                        return {};
                    }
                }
        );
        LOG_VAR(opt)

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
