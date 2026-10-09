#pragma once

#include <cstddef>

namespace fengine::ecs {

    enum team {
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
