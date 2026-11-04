#pragma once

#include <r-type/packets/Packet.hpp>
#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::client {
    class ClientPacketBuilder : public packets::PacketBuilder {
        public:
            ClientPacketBuilder();
            ~ClientPacketBuilder() = default;

            packets::PacketBuilderContent create(packets::PacketType type);
    };
}
