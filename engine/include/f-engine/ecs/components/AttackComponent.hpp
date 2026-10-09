#pragma once

#include <cstddef>

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
    };
}
