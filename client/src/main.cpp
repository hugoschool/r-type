#include "Client.hpp"
#include <f-engine/HelloWorld.hpp>
#include <iostream>
#include <string>

int main(int ac, char *av[]) {
    rtype::client::Client client;

    if (ac != 2) {
        client.helper.invalidArguments(ac);
        return 84;
    }

    std::string argument(av[1]);
    if (argument.compare("-h") == 0 || argument.compare("--help") == 0) {
        client.helper.help();
        return 0;
    }
    std::cout << "else" << std::endl;
    return 0;
}
