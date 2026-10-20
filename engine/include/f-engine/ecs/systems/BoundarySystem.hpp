#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"

namespace fengine::ecs {
    class BoundarySystem : public ASystem {
        public:
            BoundarySystem();
            ~BoundarySystem() override;

            void update(Registry &manager) override;
    };
}
