#include "protocols/UDPProtocol.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

int main(void) {
    try {
        boost::asio::io_context io_context(std::thread::hardware_concurrency());
        rtype::server::UDPProtocol server(io_context, 25000);
        io_context.run();
    } catch (std::exception &e) { std::cerr << e.what() << std::endl; }

    return 0;
}
