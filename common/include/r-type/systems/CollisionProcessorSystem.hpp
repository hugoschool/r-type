#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/ecs/systems/ISystem.hpp"
#include "r-type/components/EntityTypeComponent.hpp"

namespace fengine::ecs {
    class CollisionProcessorSystem : public ASystem {
        public:
            CollisionProcessorSystem();
            ~CollisionProcessorSystem() override;

            void update(Registry &manager) override;

        private:
            bool verifyBonusCollision(EntityTypeComponent &type, EntityTypeComponent &otherType);
    };
}
