#pragma once

#include <chrono>

namespace fengine::ecs {

    enum class behaviour {
        SHOOTER = 3000,
    };

    struct BehaviourComponent {
            enum behaviour behaviour;
            std::chrono::steady_clock::time_point behaviourClock;
    };
}
