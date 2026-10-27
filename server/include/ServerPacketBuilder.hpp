#pragma once

#include "r-type/packets/Packet.hpp"
#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::server {
    class ServerPacketBuilder : public packets::PacketBuilder {
        public:
            ServerPacketBuilder(std::size_t &tick);
            ~ServerPacketBuilder() = default;

            packets::PacketBuilderContent create(packets::PacketType type);

        private:
            std::size_t &_tick;
    };
}
