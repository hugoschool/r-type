#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"

namespace freakytd {
    class KillingSystem : public fengine::ecs::ASystem {
        public:
            KillingSystem(int &);
            ~KillingSystem();

            void update(fengine::ecs::Registry &manager) override;

        private:
            int &_money;
    };
}
