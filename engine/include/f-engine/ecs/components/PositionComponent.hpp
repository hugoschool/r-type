#pragma once

namespace fengine::ecs {
    struct PositionComponent {
            float pos_x;
            float pos_y;

            bool can_be_out_window;
    };
}
