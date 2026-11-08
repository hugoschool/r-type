#include "systems/EnnemyGenerationSystem.hpp"
#include "components/UnitComponent.hpp"
#include "Orchestrator.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"

freakytd::EnnemyGenerationSystem::EnnemyGenerationSystem(std::int64_t count, std::chrono::seconds timer, std::set<std::string> &types, Tile &tile, std::int64_t &total):
    _count(count), _timer(timer), _types(types), _beginTile(tile), _clock(std::chrono::steady_clock::now()), _total(total) {}

freakytd::EnnemyGenerationSystem::~EnnemyGenerationSystem() {}

void freakytd::EnnemyGenerationSystem::update(fengine::ecs::Registry &manager) {

    if (_total >= _count)
        return;

    std::chrono::duration<double> duration = std::chrono::steady_clock::now() - _clock;

    if (duration < _timer)
        return;
    // handle les types pour faire des ennemis random + preset ?
    auto e = manager.entityManager.spawnEntity();
    manager.componentManager.addComponent(e, (fengine::ecs::PositionComponent) {static_cast<float>(_beginTile.getX() * 50), static_cast<float>(_beginTile.getY() * 50), false});
    manager.componentManager.addComponent(e, (fengine::ecs::DrawableComponent) {"./assets/textures/Snake_walk.png", std::make_shared<fengine::modules::display::SFMLCircle>(ENNEMY_SIZE), std::nullopt});
    manager.componentManager.addComponent(e, (fengine::ecs::AnimationComponent) {48, 48, 0, 0, 4, 1, false, true, std::chrono::steady_clock::now()});
    manager.componentManager.addComponent(e, (UnitComponent) {UnitTeam::ENNEMIES, 100, std::pair(5, 8), std::pair(0, 0), 2, 25, std::chrono::steady_clock::now()});
    manager.componentManager.addComponent(e, (fengine::ecs::VelocityComponent) {0, 0, false});
    _total++;
    _clock = std::chrono::steady_clock::now();
}
