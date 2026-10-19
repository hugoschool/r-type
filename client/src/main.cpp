#include "Client.hpp"
#include "ClientHelper.hpp"
#include <iostream>

int main(int ac, char *av[]) {
    try {
        rtype::client::ClientHelper helper;

        if (helper.checkHelp(ac, av) == true) {
            return 0;
        }

        helper.verifyArgumentCount(ac);

        rtype::client::Client client;
        client.run(av);
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
