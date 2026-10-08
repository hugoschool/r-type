#pragma once

#include <iostream>

namespace fengine {
    class HelloWorld {
        public:
            HelloWorld() {
                std::cout << "Hello world from Engine!" << std::endl;
            }
    };
}
