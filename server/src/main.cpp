#include "Server.hpp"
#include "r-type/proto/packets.pb.h"
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>

int main(void) {
    try {
        GOOGLE_PROTOBUF_VERIFY_VERSION;

        rtype::server::Server server(25000);

        server.run();

        google::protobuf::ShutdownProtobufLibrary();
        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
