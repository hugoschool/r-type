#pragma once

#include "Tile.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include <chrono>
#include <set>
#include <string>

namespace freakytd {
    class EnnemyGenerationSystem : public fengine::ecs::ASystem {
        public:
            EnnemyGenerationSystem(std::int64_t, std::chrono::seconds, std::set<std::string> &, Tile &, std::int64_t &);
            ~EnnemyGenerationSystem();

            void update(fengine::ecs::Registry &manager) override;

        private:
            std::int64_t _count;
            std::chrono::seconds _timer;
            std::set<std::string> &_types;
            Tile &_beginTile;

            std::chrono::time_point<std::chrono::steady_clock> _clock;
            std::int64_t &_total;
    };
}
