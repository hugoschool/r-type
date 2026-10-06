#include "Client.hpp"

rtype::client::Client::Client() {
}

rtype::client::Client::~Client() {
}

int rtype::client::Client::run(char *av[]) {
    if (this->connect(av) == 84) {
        return 84;
    }

    this->loop();
    return 0;
}

int rtype::client::Client::connect(char *av[]) {
    // TODO
    return 0;
}

int rtype::client::Client::loop() {
    while (true) {
        // get player input
        // send it to the server
        // create prediction based on the input
        // get server response compare server response with the predition
    }
    return 0;
}
