#pragma once

#include "Exception.hpp"
#include <cstdint>
#include <iostream>
#include <string>

namespace rtype {
    namespace server {
        class ServerHelper {
            private:
                static std::string invalidArguments(int argumentNumber) {
                    return "Invalid arguments: given " + std::to_string(argumentNumber - 1) + ", expected 1 none.";
                }

                static void help() {
                    std::cout << "r-type_server: Server for the r-type game" << std::endl;
                    std::cout << std::endl;
                    std::cout << "Usage:" << std::endl;
                    std::cout << "r-type_server [port]" << std::endl;
                    std::cout << "port: Port on which the game will be hosted" << std::endl;
                    std::cout << std::endl;
                    std::cout << "If no port is provided the default is to 25000." << std::endl;
                }

            public:
                static void verifyArgumentCount(int ac) {
                    if (ac > 2) {
                        throw ServerException(invalidArguments(ac));
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

                static std::uint32_t getPort(int ac, char *av[]) {
                    if (ac == 1) {
                        return 25000;
                    } else {
                        std::uint32_t port = std::stoul(av[1]);
                        return port;
                    }
                }
        };
    }
}
