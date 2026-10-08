#pragma once

#include <cstdint>

namespace fengine {
    namespace ecs {
        using EntityId = std::size_t;

        class Entity {
            public:
                explicit Entity(EntityId id) : _id(id) {};
                ~Entity() {};

                explicit operator std::size_t() {
                    return _id;
                };

                EntityId getId() const {
                    return _id;
                };

                bool operator==(const Entity &other) const {
                    return _id == other._id;
                }

            private:
                EntityId _id;
        };
    }
}
