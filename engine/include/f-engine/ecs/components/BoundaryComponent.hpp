#pragma once

#include <chrono>

namespace fengine::ecs {
    struct BoundaryComponent {
        float size_x;
        float size_y;

        bool hittable;
        std::optional<std::chrono::time_point<std::chrono::steady_clock>> can_be_hit;
    };
}
