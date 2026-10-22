#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"

namespace fengine::ecs {
    class BehaviouralSystem : public ASystem {
        public:
            BehaviouralSystem();
            ~BehaviouralSystem() override;

            void update(Registry &manager) override;
    };
}
