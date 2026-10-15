#pragma once

#include <cstddef>

namespace fengine::ecs {
    struct DeadComponent {
        std::size_t dead_id;
    };
}
