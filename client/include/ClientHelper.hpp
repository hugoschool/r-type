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
                    std::cout << "r-type_client: Client for the r-type game" << std::endl;
                    std::cout << std::endl;
                    std::cout << "Usage:" << std::endl;
                    std::cout << "r-type_client [host] [port]" << std::endl;
                    std::cout << "host: IP address of the game host" << std::endl;
                    std::cout << "port: Port of the game host" << std::endl;
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
