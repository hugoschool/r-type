#pragma once

#include <cstddef>

namespace fengine::ecs {
    class Entity {
        public:
            Entity(std::size_t id);
            ~Entity();

            operator std::size_t();

            std::size_t getId() const;

            bool operator==(const Entity &other) const;

        private:
            std::size_t _id;
    };
}
