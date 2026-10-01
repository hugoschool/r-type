#pragma once

#include <iostream>

namespace rtype {
    namespace engine {
        class HelloWorld {
            public:
                HelloWorld() {
                    std::cout << "Hello world from Engine!" << std::endl;
                }
        };
    }
}
