#include "Client.hpp"
#include "ClientHelper.hpp"
#include "r-type/proto/packets.pb.h"
#include <iostream>

int main(int ac, char *av[]) {
    try {
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        rtype::client::ClientHelper helper;

        if (helper.checkHelp(ac, av) == true) {
            return 0;
        }

        helper.verifyArgumentCount(ac);

        rtype::client::Client client(av[1], av[2]);
        client.run();

        google::protobuf::ShutdownProtobufLibrary();
    } catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
