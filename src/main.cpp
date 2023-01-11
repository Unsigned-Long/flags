#include "flags.hpp"
#include "artwork/logger/logger.h"

int main(int argc, char const *argv[]) {
    try {
        using namespace ns_flags;
        std::shared_ptr<Argument> arg = std::make_shared<IntVec>();

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
