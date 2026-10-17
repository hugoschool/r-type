#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

namespace rtype::client {
    class Client {
        public:
            using udp = boost::asio::ip::udp;
            Client();

            void connect();
            void run();

        private:
            boost::asio::io_context _io_context;
            udp::resolver _resolver;
            udp::endpoint _endpoint;
            udp::socket _socket;

            std::string _uuid;
    };
}
