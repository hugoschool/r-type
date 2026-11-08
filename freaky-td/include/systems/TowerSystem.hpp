#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include <random>

namespace freakytd {
    class TowerSystem : public fengine::ecs::ASystem {
        public:
            TowerSystem();
            ~TowerSystem();

            void update(fengine::ecs::Registry &manager) override;

        private:
            float randomFloat(std::pair<float, float>);

            std::default_random_engine _gen;
    };
}
