#pragma once

#include <chrono>

namespace fengine::ecs {
    struct AnimationComponent {
        int length;
        int height;
        int offset_x;
        int offset_y;
        int cols;
        int rows;

        std::chrono::time_point<std::chrono::steady_clock> anim_clock;
    };
}
