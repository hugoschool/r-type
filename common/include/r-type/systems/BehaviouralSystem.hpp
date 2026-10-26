#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"

namespace fengine::ecs {
    class BehaviouralSystem : public ASystem {
        // Will be added here a reference to the sound module, for when we need to play sounds based on behaviour
        public:
            BehaviouralSystem();
            ~BehaviouralSystem() override;

            void update(Registry &manager) override;
    };
}
