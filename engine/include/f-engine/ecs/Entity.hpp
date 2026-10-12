#pragma once

#include <cstdint>

namespace fengine::ecs {
    class Entity {
        public:
            explicit Entity(std::size_t id) : _id(id) {};
            ~Entity() {};

            explicit operator std::size_t() {
                return _id;
            };

            std::size_t getId() const {
                return _id;
            };

            bool operator==(const Entity &other) const {
                return _id == other._id;
            }

        private:
            std::size_t _id;
    };
}
