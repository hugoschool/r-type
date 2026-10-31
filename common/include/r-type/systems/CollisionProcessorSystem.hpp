#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"

namespace fengine::ecs {
    class CollisionProcessorSystem : public ASystem {
        public:
            CollisionProcessorSystem();
            ~CollisionProcessorSystem() override;

            void update(Registry &manager) override;
    };
}
