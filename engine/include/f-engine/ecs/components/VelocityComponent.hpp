#pragma once

namespace fengine::ecs {
    struct VelocityComponent {
            float vel_x;
            float vel_y;
    };

    VelocityComponent operator-(VelocityComponent &vel) {
        return VelocityComponent(-vel.vel_x, -vel.vel_y);
    };
}
