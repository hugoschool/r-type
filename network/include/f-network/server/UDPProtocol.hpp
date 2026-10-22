#pragma once

#include "AProtocol.hpp"
#include <boost/asio/ip/udp.hpp>

namespace fnetwork::server {
    class UDPProtocol : public AProtocol {
        public:
            using udp = boost::asio::ip::udp;

            UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port);
            ~UDPProtocol() override = default;

            void startReceive();
            virtual void handleReceive(const boost::system::error_code &error, std::size_t) = 0;

            void handleSend(std::shared_ptr<std::string>, const boost::system::error_code &, std::size_t);
            void run() final;

        protected:
            udp::socket _socket;
            udp::endpoint _endpoint;
    };
}
