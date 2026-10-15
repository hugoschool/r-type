#pragma once

#include <cstddef>

namespace fengine::ecs {
    struct DeadComponent {
        public:
            std::size_t entityId;
    };
}
