#pragma once

#include <iostream>

namespace rtype {
    namespace client {
        class HelloWorld {
            public:
                HelloWorld() {
                    std::cout << "Hello world from client!" << std::endl;
                }
        };
    }
}
