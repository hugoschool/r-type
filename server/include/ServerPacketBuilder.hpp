#pragma once

#include "TickHandler.hpp"
#include "r-type/packets/Packet.hpp"
#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::server {
    class ServerPacketBuilder : public packets::PacketBuilder {
        public:
            ServerPacketBuilder(TickHandler &tick);
            ~ServerPacketBuilder() = default;

            packets::PacketBuilderContent create(packets::PacketType type);

        private:
            TickHandler &_tick;
    };
}
