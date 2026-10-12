#pragma once

#include "PacketType.hpp"
#include <cstdint>
#include <boost/serialization/string.hpp>

namespace rtype::packets {
    struct Packet {
        std::uint64_t id;
        packets::PacketType command;
        std::string uuid;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & id;
            ar & command;
            ar & uuid;
        }
    };
}
