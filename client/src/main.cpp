#include "Client.hpp"
#include "Exception.hpp"
#include <iostream>


int main(int ac, char *av[]) {
    try {
        rtype::client::Client client;

        if (client.helper.checkHelp(ac, av) == true) {
            return 0;
        }

        client.helper.verifyArgumentCount(ac);

        client.run(av);
    } catch (rtype::client::ClientException & exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
