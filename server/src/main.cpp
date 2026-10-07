#include "Server.hpp"
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>

int main(void) {
    try {
        rtype::server::Server server(25000);

        server.run();

        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
