#include "protocols/UDPProtocol.hpp"
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <thread>

int main(void) {
    try {
        unsigned int thread_amount = std::thread::hardware_concurrency();
        boost::asio::io_context io_context(thread_amount);
        boost::asio::thread_pool pool(thread_amount);

        rtype::server::UDPProtocol server(io_context, 25000);

        for (unsigned int i = 0; i < thread_amount; i++) {
            boost::asio::post(pool, [&server]() { server.run(); });
        }
        pool.join();

        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
