#pragma once

#include <cstddef>

namespace rtype::packets {
    enum class PacketType {
        UpdatePlayer,
        UpdateEntity,
        KillPlayer,
        KillEntity,
        AddPlayer,
        AddEntity,
        DeadPlayer,
        Ack,
    };

    struct PacketHeader {
        public:
            PacketType type;
            std::size_t size;

            template <class Archive>
            void serialize(Archive &ar, const unsigned int) {
                ar & type;
                ar & size;
            }
    };
}
