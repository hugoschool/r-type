#pragma once

#include <iostream>

namespace rtype {
    namespace client {
        class ClientHelper {
            public:
                static void invalidArguments(int argumentNumber) {
                    std::cout << "Invalid arguments: given " << argumentNumber - 1 << ", expected 1." << std::endl;
                }
                static void help() {
                    // TODO write a valid help message
                    std::cout << "help message" << std::endl;
                }
        };
    }
}
