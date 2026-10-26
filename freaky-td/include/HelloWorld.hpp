#pragma once

#include <iostream>

namespace freakytd {
    class HelloWorld {
        public:
            HelloWorld() {
                std::cout << "Hello world!" << std::endl;
            };
            ~HelloWorld() = default;
    };
}
