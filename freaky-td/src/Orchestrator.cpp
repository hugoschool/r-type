#include "Orchestrator.hpp"
#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/systems/AnimationSystem.hpp"
#include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include "map_builder/SFMLMapBuilder.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/ecs/systems/MoveSystem.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/sound/SFMLSoundModule.hpp"
#include "map_builder/SFMLMapBuilder.hpp"
#include "components/TileComponent.hpp"
#include "components/TowerComponent.hpp"
#include "components/UnitComponent.hpp"
#include "systems/TowerSystem.hpp"
#include "systems/KillingSystem.hpp"
#include "systems/EnnemyPathfindingSystem.hpp"
#include "systems/EnnemyGenerationSystem.hpp"
#include <chrono>
#include <iostream>
#include <memory>

freakytd::Orchestrator::Orchestrator(std::string filepath): _world(fengine::world::World(0, 10000)), _mapBuilder(std::make_unique<SFMLMapBuilder>(_world._registry)),
    _assetManager(std::make_shared<fengine::asset_manager::SFMLAssetManager>()),
    _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>(std::static_pointer_cast<fengine::asset_manager::SFMLAssetManager>(_assetManager), "Freaky TD")),
    _soundModule(std::make_unique<fengine::modules::sound::SFMLSoundModule>(std::static_pointer_cast<fengine::asset_manager::SFMLAssetManager>(_assetManager))),
    _levels(), _ennemySpawnClock(std::chrono::steady_clock::now()), _enemyTotal(0), _lives(0), _money(500), _scene(std::make_shared<GameScene>("NONE", false, _money, _lives)) {

    _world._registry.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::DrawableComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::AnimationComponent>();
    _world._registry.componentManager.registerComponent<TileComponent>();
    _world._registry.componentManager.registerComponent<TowerComponent>();
    _world._registry.componentManager.registerComponent<UnitComponent>();

    _world._registry.systemManager.addSystem<fengine::ecs::MovementSystem>(_world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::DisplaySystem>(*_displayModule, _world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::AnimationSystem>(*_displayModule, _world._camera);
    _world._registry.systemManager.addSystem<freakytd::TowerSystem>();
    _world._registry.systemManager.addSystem<freakytd::KillingSystem>(_money);

    try {
        _levels.parseFile(filepath);
    } catch (std::exception const &e) {
        throw e;
    }

    generateCurrentLevel();
}

freakytd::Orchestrator::~Orchestrator() {}

void freakytd::Orchestrator::generateCurrentLevel() {
    _enemyTotal = 0;

    auto &level = _levels.getCurrentLevel().value().get();
    auto &paths = level.getPaths();
    _lives = _levels.getCurrentLevel().value().get().getPlayerHP();

    _mapBuilder->generateTiles(paths);
    _world._registry.systemManager.addSystem<freakytd::EnnemyPathfindingSystem>(paths[0], _lives);
    _world._registry.systemManager.addSystem<freakytd::EnnemyGenerationSystem>(level.getEntityTotalAmount(), level.getEntitySpawnRate(), level.getEntityTypes(), paths[0].getStart(), _enemyTotal);
}

void freakytd::Orchestrator::refundTowers() {
    auto &towers = _world._registry.componentManager.getComponents<TowerComponent>();

    for (std::size_t i = 0; i < _world._registry.entityManager.size(); i++) {
        auto &entity = _world._registry.entityManager.entityFromIndex(i);
        if (!entity.has_value())
            continue;

        auto &tower = towers[i];

        if (!tower.has_value())
            continue;

        switch (tower->towerType) {
            case TowerType::ARCHER:
                _money += 100;
                break;
            case TowerType::MAGICIAN:
                _money += 150;
                break;
            default:
                break;
        }

        tower = std::nullopt;
        _world._registry.entityManager.killEntity(entity.value());
    }
}

void freakytd::Orchestrator::verifyLevel() {
    auto &level = _levels.getCurrentLevel().value().get();

    if (_enemyTotal < level.getEntityTotalAmount()) {
        return;
    }

    auto &units = _world._registry.componentManager.getComponents<UnitComponent>();

    for (std::size_t i = 0; i < _world._registry.entityManager.size(); i++) {
        if (!_world._registry.entityManager.entityFromIndex(i).has_value())
            continue;

        auto &unit = units[i];

        if (!unit.has_value() || unit->team != UnitTeam::ENNEMIES)
            continue;

        if (unit->health != 0)
            return;
    }

    refundTowers();

    auto &tiles = _world._registry.componentManager.getComponents<TileComponent>();

    for (std::size_t i = 0; i < _world._registry.entityManager.size(); i++) {
        auto &entity = _world._registry.entityManager.entityFromIndex(i);
        if (!entity.has_value())
            continue;

        auto &tile = tiles[i];

        if (!tile.has_value())
            continue;

        tile->type = TileType::BASIC;
    }

    _world._registry.systemManager.removeSystem<freakytd::EnnemyPathfindingSystem>();
    _world._registry.systemManager.removeSystem<freakytd::EnnemyGenerationSystem>();

    _levels.nextLevel();

    generateCurrentLevel();
}

void freakytd::Orchestrator::loop() {
    bool exit = false;
    while (!exit) {
        _displayModule->clear();

        _world._registry.systemManager.update(_world._registry);
        _scene->update();
        _displayModule->drawScene(_scene);

        // TODO: this can just be a queue, optional makes no sense here
        std::optional<std::queue<std::unique_ptr<fengine::modules::display::IEvent>>> events =
            _displayModule->pollEvent();
        if (events.has_value()) {
            while (events.value().empty() == false) {
                std::unique_ptr<fengine::modules::display::IEvent> event = std::move(events.value().front());
                events.value().pop();
                if (event->getType() == fengine::modules::display::EventType::Quit) {
                    exit = true;
                    break;
                }
                if (event->getType() == fengine::modules::display::EventType::Click) {
                    handleClickEvent(event->getClickCoordinates(), event->getMouseButton());
                }
            }
        }
        if (_lives <= 0) {
            std::cout << "No HP left, game lost!" << std::endl;
            exit = true;
        }
        verifyLevel();
    }
}

void freakytd::Orchestrator::handleClickEvent(std::pair<float, float> &clickCoords, fengine::modules::display::EventMouseButton mouseButton) {
    auto &tileTypes = _world._registry.componentManager.getComponents<TileComponent>();
    auto &positions = _world._registry.componentManager.getComponents<fengine::ecs::PositionComponent>();

    for (std::size_t i = 0; i < _world._registry.entityManager.size(); i++) {
        auto &type = tileTypes[i];
        auto &pos = positions[i];

        if (!pos.has_value())
            continue;
        if (!type.has_value())
            break;

        bool inTile = (pos.value().pos_x < clickCoords.first
                        && pos.value().pos_x + 50 > clickCoords.first
                        && pos.value().pos_y< clickCoords.second
                        && pos.value().pos_y+ 50 > clickCoords.second);

        if (type.value().type == TileType::BASIC && inTile) {
            auto e = _world._registry.entityManager.spawnEntity();
            type.value().type = TileType::TOWER;
            _world._registry.componentManager.addComponent(e, (fengine::ecs::PositionComponent) {pos.value().pos_x, pos.value().pos_y, false});
            if (mouseButton == fengine::modules::display::EventMouseButton::Left && _money >= 100) {
                _world._registry.componentManager.addComponent(e, (TowerComponent) {std::pair(8, 15), 150, DamageType::PHYSICAL, TowerType::ARCHER, 750, std::chrono::steady_clock::now()});
                _world._registry.componentManager.addComponent(e, (fengine::ecs::DrawableComponent) {"./assets/textures/archer.png", std::make_shared<fengine::modules::display::SFMLRectangle>(50, 50), std::nullopt});
                _money -= 100;
            }
            if (mouseButton == fengine::modules::display::EventMouseButton::Right && _money >= 150) {
                _world._registry.componentManager.addComponent(e, (TowerComponent) {std::pair(15, 25), 175, DamageType::MAGICAL, TowerType::MAGICIAN, 1150, std::chrono::steady_clock::now()});
                _world._registry.componentManager.addComponent(e, (fengine::ecs::DrawableComponent) {"./assets/textures/magician.png", std::make_shared<fengine::modules::display::SFMLRectangle>(50, 50), std::nullopt});
                _money -= 150;
            }
        } else if (type.value().type == TileType::PATH && inTile) {
            auto e = _world._registry.entityManager.spawnEntity();
            _world._registry.componentManager.addComponent(e, (fengine::ecs::PositionComponent) {clickCoords.first, clickCoords.second, false});
            _world._registry.componentManager.addComponent(e, (fengine::ecs::DrawableComponent) {"./assets/textures/Snake_walk.png", std::make_shared<fengine::modules::display::SFMLCircle>(ENNEMY_SIZE), std::nullopt});
            _world._registry.componentManager.addComponent(e, (fengine::ecs::AnimationComponent) {48, 48, 0, 0, 4, 1, false, true, std::chrono::steady_clock::now()});
            _world._registry.componentManager.addComponent(e, (UnitComponent) {UnitTeam::ENNEMIES, 100, std::pair(5, 8), std::pair(0, 0), 2, 25, std::chrono::steady_clock::now()});
            _world._registry.componentManager.addComponent(e, (fengine::ecs::VelocityComponent) {0, 0, false});
        }

    }
}
