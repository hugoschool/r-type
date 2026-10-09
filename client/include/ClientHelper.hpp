#pragma once

#include "Exception.hpp"
#include <iostream>
#include <string>

namespace rtype {
    namespace client {
        class ClientHelper {
            private:
                static std::string invalidArguments(int argumentNumber) {
                    return "Invalid arguments: given " + std::to_string(argumentNumber - 1) + ", expected 2.";
                }

                static void help() {
                    // TODO write a valid help message
                    std::cout << "Help message." << std::endl;
                }

            public:
                static void verifyArgumentCount(int ac) {
                    if (ac != 3) {
                        throw ClientException(invalidArguments(ac));
                    }
                }

                static bool checkHelp(int ac, char *av[]) {
                    if (ac != 2) {
                        return false;
                    }

                    std::string flag(av[1]);

                    if (flag.compare("-h") == 0 || flag.compare("--help") == 0) {
                        help();
                        return true;
                    } else {
                        return false;
                    }
                }
        };
    }
}
