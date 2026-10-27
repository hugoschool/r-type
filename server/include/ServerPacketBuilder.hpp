#pragma once

#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::server {
    class ServerPacketBuilder : public packets::PacketBuilder {
        public:
            ServerPacketBuilder();
            ~ServerPacketBuilder() = default;
    };
}
