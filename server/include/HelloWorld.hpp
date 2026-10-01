#pragma once

#include <iostream>

namespace rtype {
    namespace server {
        class HelloWorld {
            public:
                HelloWorld() {
                    std::cout << "Hello world from server!" << std::endl;
                }
        };
    }
}
