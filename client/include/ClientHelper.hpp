#pragma once

#include <iostream>

namespace rtype {
    namespace client {
        class ClientHelper {
            private:
                static void invalidArguments(int argumentNumber) {
                    std::cout << "Invalid arguments: given " << argumentNumber - 1 << ", expected 2." << std::endl;
                }

                static void help() {
                    // TODO write a valid help message
                    std::cout << "Help message." << std::endl;
                }

            public:
                static int verifyArgumentCount(int ac) {
                    if (ac != 3) {
                        invalidArguments(ac);
                        return 84;
                    } else {
                        return 0;
                    }
                }

                static int checkHelp(int ac, char *av[]) {
                    if (ac != 2) {
                        return 0;
                    }

                    std::string flag(av[1]);

                    if (flag.compare("-h") == 0 || flag.compare("--help") == 0) {
                        help();
                        return 1;
                    } else {
                        return 0;
                    }
                }
        };
    }
}
