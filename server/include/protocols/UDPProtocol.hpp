#pragma once

#include "protocols/IProtocol.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstdint>

namespace rtype::server {
    class UDPProtocol : public IProtocol {
        public:
            using udp = boost::asio::ip::udp;

            UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port);
            ~UDPProtocol() override = default;

            void startReceive();
            void handleReceive(const boost::system::error_code &error, std::size_t);
            void handleSend(std::shared_ptr<std::string>, const boost::system::error_code &, std::size_t);
            void run() final;

        private:
            boost::asio::io_context &_io_context;

            std::uint16_t _port;
            udp::socket _socket;
            udp::endpoint _endpoint;

            std::array<char, 1024> _buffer;
    };
}
