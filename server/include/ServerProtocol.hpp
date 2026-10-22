#pragma once

#include "ClientHandler.hpp"
#include "SafeQueue.hpp"
#include "f-network/server/UDPProtocol.hpp"

namespace rtype::server {
    class ServerProtocol : public fnetwork::server::UDPProtocol {
        public:
            ServerProtocol(boost::asio::io_context &io_context, std::uint16_t port, SafeMessageQueue &SafeMessageQueue,
                ClientHandler &clientHandler);
            ~ServerProtocol() override = default;

            void handleReceive(const boost::system::error_code &error, std::size_t) final;

        private:
            SafeMessageQueue &_messageQueue;
            ClientHandler &_clientHandler;
    };
}
