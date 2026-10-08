#pragma once

#include <cstddef>
#include <string>

namespace rtype::packets {
    struct Vector2D {
            double x;
            double y;
    };

    struct Player {
            std::string uuid;
            Vector2D position;
            Vector2D velocity;
            int health;
    };

    struct Entity {
            std::size_t id;
            Vector2D position;
            Vector2D velocity;
            int health;
    };

    union EntityType {
            Player player;
            Entity entity;
    };
}
