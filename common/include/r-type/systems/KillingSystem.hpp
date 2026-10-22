#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"

namespace fengine::ecs {
    class KillingSystem : public ASystem {
        public:
            KillingSystem();
            ~KillingSystem() override;

            void update(Registry &manager) override;
    };
}
