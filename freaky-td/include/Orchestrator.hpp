#pragma once

#include "f-engine/asset_manager/IAssetManager.hpp"
#include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/scene/IScene.hpp"
#include "levels/Levels.hpp"
#include "map_builder/IMapBuilder.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/sound/ISoundModule.hpp"
#include "f-engine/world/World.hpp"
#include "map_builder/IMapBuilder.hpp"
#include "scenes/GameScene.hpp"
#include <chrono>
#include <memory>
#include <utility>

namespace freakytd {

    constexpr int ENNEMY_SIZE = 50;

    class Orchestrator {
        public:
            Orchestrator(std::string);
            ~Orchestrator();

            void loop();
        private:
            fengine::world::World _world;
            std::unique_ptr<IMapBuilder> _mapBuilder;
            std::shared_ptr<fengine::asset_manager::IAssetManager> _assetManager;
            std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
            std::unique_ptr<fengine::modules::sound::ISoundModule> _soundModule;

            void handleClickEvent(std::pair<float, float> &clickEvent, fengine::modules::display::EventMouseButton);
            void handleEnnemySpawn(void);

            Levels _levels;
            std::chrono::time_point<std::chrono::steady_clock> _ennemySpawnClock; // make a vector for this in case there are multiple ennmy type to make things a little more funky

            std::int64_t _enemyTotal;
            int _lives;
            int _money;

            std::shared_ptr<fengine::scene::IScene> _scene;

            void generateCurrentLevel();
            void verifyLevel();
            void refundTowers();
    };
}
