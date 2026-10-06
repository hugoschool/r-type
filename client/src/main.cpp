#include "Client.hpp"
#include <iostream>
#include <f-engine/HelloWorld.hpp>

int main(void) {
    try {
        rtype::client::Client client;

        if (client.helper.checkHelp(ac, av) == 1) {
            return 0;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
