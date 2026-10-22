#pragma once

#include "AProtocol.hpp"
#include <boost/asio/ip/udp.hpp>

namespace fnetwork::client {
    class UDPProtocol : public AProtocol {
        public:
            using udp = boost::asio::ip::udp;

            UDPProtocol(boost::asio::io_context &_io_context, std::string host, std::uint16_t port);
            ~UDPProtocol() override = default;

            void connect() override;
            void close() override;

            void send(std::string content) override;
            std::string read(boost::system::error_code &error) override;
            void setSocketNonBlocking() override;

        private:
            udp::resolver _resolver;
            udp::endpoint _endpoint;
            udp::socket _socket;
    };
}
