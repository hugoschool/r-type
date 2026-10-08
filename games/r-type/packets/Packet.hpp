#pragma once

#include "Entity.hpp"
#include "PacketType.hpp"
#include <cstdint>

namespace rtype::packets {
    struct Packet {
            std::uint64_t id;
            packets::PacketType command;
            std::string uuid;
            EntityType msg;
    };
}
