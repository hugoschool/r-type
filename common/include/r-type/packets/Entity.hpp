#pragma once

#include <cstddef>
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

    struct Player {
        public:
            std::string uuid;
            Vector2D position;
            Vector2D velocity;
            int health;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & uuid;
                ar & position;
                ar & velocity;
                ar & health;
            }
    };

    struct Entity {
        public:
            std::size_t id;
            Vector2D position;
            Vector2D velocity;
            int health;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & id;
                ar & position;
                ar & velocity;
                ar & health;
            }
    };

    struct EntityID {
        public:
            std::size_t entityId;

            template <class Archive>
            void serialize(Archive &ar, unsigned int) {
                ar & entityId;
            }
    };

    struct Empty {
        public:
            template <class Archive>
            void serialize(Archive &, unsigned int) {
            }
    };
}
