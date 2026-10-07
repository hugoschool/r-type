#pragma once

#include "protocols/UDPProtocol.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <cstdint>

namespace rtype::server {
    class Server {
        public:
            explicit Server(std::uint32_t port);
            ~Server();

            void run();

        private:
            std::uint32_t _port;

            boost::asio::io_context _io_context;

            unsigned int _threadAmount;
            boost::asio::thread_pool _threadPool;

            UDPProtocol _udp;
    };
}
