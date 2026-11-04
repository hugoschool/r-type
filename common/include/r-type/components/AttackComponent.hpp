#pragma once

#include <chrono>
#include <cstddef>
#include <optional>

namespace fengine::ecs {
    enum class team {
        ALLIES,
        MONSTERS,
    };

    union relation {
            std::size_t related_id;
            enum team team;
    };

    struct AttackComponent {
            union relation relation;
            float damage;

            std::optional<std::chrono::time_point<std::chrono::steady_clock>> _shootCooldown;
    };
}
