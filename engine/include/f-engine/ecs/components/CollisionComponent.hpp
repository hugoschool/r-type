#pragma once

#include <cstddef>

namespace fengine::ecs {
    struct CollisionComponent {
            std::size_t self_id;
            std::size_t other_id;

            bool set;
    };
}
