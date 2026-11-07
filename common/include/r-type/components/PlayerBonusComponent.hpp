#pragma once

#include "r-type/presets/BonusTypes.hpp"
#include <chrono>

namespace fengine::ecs {
    struct PlayerBonusComponent {
            presets::BonusTypes type;
            std::chrono::time_point<std::chrono::steady_clock> _bonusDuration;
            bool _shouldExpire;
    };
}
