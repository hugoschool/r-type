#pragma once

#include "TickHandler.hpp"
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
            void tickThreadRun();

        private:
            std::uint32_t _port;

            boost::asio::io_context _io_context;

            unsigned int _totalThreadAmount;
            unsigned int _protocolThreadAmount;
            boost::asio::thread_pool _threadPool;

            TickHandler _tickHandler;

            UDPProtocol _udp;
    };
}
