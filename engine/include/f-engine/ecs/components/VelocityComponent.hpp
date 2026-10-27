#pragma once

namespace fengine::ecs {
    struct VelocityComponent {
            float vel_x;
            float vel_y;

            bool can_stop;
    };

    inline VelocityComponent operator-(VelocityComponent &vel) {
        return VelocityComponent(-vel.vel_x, -vel.vel_y, vel.can_stop);
    };
}
