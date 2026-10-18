#pragma once

#include <cstddef>

namespace rtype::packets {
    // Size of a UUID v4 string with dashes
    constexpr std::size_t UUID_SIZE = 36;

    enum class PacketType {
        LoginRequest,
        LoginAnswer,
        LoginFailed,
        EntitySpawn,
        EntityMove,
        EntityVelocityChange,
        EntityDied,
        Ack,
        InputPlayer
    };

    struct PacketHeader {
        public:
            PacketType type;
            std::size_t size;
            char playerUuid[UUID_SIZE + 1];

            template <class Archive>
            void serialize(Archive &ar, const unsigned int) {
                ar & type;
                ar & size;
                ar & playerUuid;
            }
    };
}
