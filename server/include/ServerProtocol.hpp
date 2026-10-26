#pragma once

#include "ClientHandler.hpp"
#include "SafeQueue.hpp"
#include "f-network/server/async/UDPProtocol.hpp"
#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::server {
    class ServerProtocol : public fnetwork::server::async::UDPProtocol {
        public:
            ServerProtocol(boost::asio::io_context &io_context, std::uint16_t port, SafeMessageQueue &SafeMessageQueue,
                ClientHandler &clientHandler, ServerPacketBuilder &packetBuilder);
            ~ServerProtocol() override = default;

            void handleReceive(const boost::system::error_code &error, std::size_t) final;

        private:
            SafeMessageQueue &_messageQueue;
            ClientHandler &_clientHandler;
            ServerPacketBuilder &_packetBuilder;
    };
}
