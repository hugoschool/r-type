#pragma once

#include "r-type/event/PlayerEvent.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/presets/EntitiesPresets.hpp"
#include <cstddef>
#include <cstring>
#include <string>

namespace rtype::packets {
    struct Vector2D {
        public:
            double x;
            double y;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & x;
                ar & y;
            }
    };

    struct LoginAnswer {
        public:
            char uuid[UUID_SIZE + 1];

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & uuid;
            }

            void setUuid(std::string newUuid) {
                std::memcpy(uuid, newUuid.c_str(), sizeof(char) * UUID_SIZE + 1);
            }
    };

    struct EntityIdOnly {
        public:
            std::size_t entityId;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & entityId;
            }
    };

    struct EntitySpawn {
        public:
            std::size_t entityId;
            fengine::presets::EntityType entityType;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & entityId;
                ar & entityType;
            }
    };

    using EntityDied = EntityIdOnly;

    struct EntityMove {
        public:
            std::size_t entityId;
            Vector2D coordinates;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & entityId;
                ar & coordinates;
            }
    };

    struct EntityVelocityChange {
        public:
            std::size_t entityId;
            Vector2D velocity;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & entityId;
                ar & velocity;
            }
    };

    struct PlayerEvent {
        public:
            rtype::event::Event event;
            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & event;
            }
    };
}
