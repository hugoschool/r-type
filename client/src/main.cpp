#include "Client.hpp"


int main(int ac, char *av[]) {
    rtype::client::Client client;

    if (client.helper.checkHelp(ac, av) == 1) {
        return 0;
    }
    if (client.helper.verifyArgumentCount(ac) == 84) {
        return 84;
    }
    if (client.run(av) == 84) {
        return 84;
    }
    return 0;
}
