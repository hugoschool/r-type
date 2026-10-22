#pragma once

#include "f-engine/ecs/systems/ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"

namespace fengine::ecs {
    class CollisionDamageSystem : public ASystem {
        public:
            CollisionDamageSystem();
            ~CollisionDamageSystem() override;

            void update(Registry &manager) override;
    };
}
