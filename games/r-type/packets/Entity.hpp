#pragma once

#include <cstddef>
#include <string>

namespace rtype::packets {
    struct Vector2D {
        double x;
        double y;
        template<class Archive>
        void serialize(Archive &ar, unsigned int) {
            ar & x;
            ar & y;
        }
    };

    struct Player {
        std::string uuid;
        Vector2D position;
        Vector2D velocity;
        int health;
        template<class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar & uuid;
            ar & position;
            ar & velocity;
            ar & health;
        }
    };

    struct Entity {
        std::size_t id;
        Vector2D position;
        Vector2D velocity;
        int health;
        template<class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar & id;
            ar & position;
            ar & velocity;
            ar & health;
        }
    };

}
