#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"

namespace fengine::ecs {
    class CollisionDetectionSystem : public ASystem {
        public:
            CollisionDetectionSystem();
            ~CollisionDetectionSystem() override;

            void update(Registry &manager) override;
    };
}
