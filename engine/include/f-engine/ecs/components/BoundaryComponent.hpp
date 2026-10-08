#pragma once

namespace fengine::ecs {
    struct BoundaryComponent {
        float size_x;
        float size_y;

        bool bounce;
        bool out_win;
    };
}
