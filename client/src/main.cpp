#include "Client.hpp"
#include <iostream>

int main(void) {
    try {
        rtype::client::Client client;

        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
