#include "Server.hpp"
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include "ServerHelper.hpp"

int main(int ac, char *av[]) {
    try {
        rtype::server::ServerHelper helper;

        if (helper.checkHelp(ac, av) == true) {
            return 0;
        }

        helper.verifyArgumentCount(ac);

        std::uint32_t port = helper.getPort(ac, av);

        rtype::server::Server server(port);
        server.run();

        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
