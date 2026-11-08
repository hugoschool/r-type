#pragma once

#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "levels/Path.hpp"

namespace freakytd {
    class EnnemyPathfindingSystem : public fengine::ecs::ASystem {
        public:
            EnnemyPathfindingSystem(Path &, int &);
            ~EnnemyPathfindingSystem();

            void update(fengine::ecs::Registry &manager) override;

        private:
            Path &_path;
            int &_lives;
    };
}
