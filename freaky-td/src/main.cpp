#include "Orchestrator.hpp"
#include <exception>
#include <f-engine/ecs/Registry.hpp>
#include <f-engine/modules/display/SFMLDisplayModule.hpp>
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Missing level argument" << std::endl;
        return 1;
    }

    try {
        freakytd::Orchestrator orch(argv[1]);
        orch.loop();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
