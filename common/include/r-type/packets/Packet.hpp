#pragma once

#include <cstddef>
#include <cstring>
#include <string>

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
        PlayerEvent,
        Level,
    };

    enum class PacketPriority {
        High,
        Low
    };

    struct PacketHeader {
        public:
            PacketType type;
            std::size_t size;
            std::size_t tick;
            char playerUuid[UUID_SIZE + 1];
            PacketPriority priority;
            size_t id;

            template <class Archive>
            void serialize(Archive &ar, const unsigned int) {
                ar & type;
                ar & size;
                ar & tick;
                ar & playerUuid;
                ar & priority;
                ar & id;
            }

            void setUuid(std::string uuid) {
                std::memcpy(playerUuid, uuid.c_str(), sizeof(char) * UUID_SIZE + 1);
            }
    };
}
