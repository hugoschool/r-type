#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"

fengine::ecs::DisplaySystem::DisplaySystem(modules::display::IDisplayModule &displayModule, camera::Camera &camera)
    : _displayModule(displayModule), _camera(camera) {};

fengine::ecs::DisplaySystem::~DisplaySystem() {};

void fengine::ecs::DisplaySystem::update(Registry &manager) {
    auto &positions = manager.componentManager.getComponents<PositionComponent>();
    auto &drawables = manager.componentManager.getComponents<DrawableComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &pos = positions[i];
        auto &drawable = drawables[i];

        if (pos.has_value() && drawable.has_value()) {
            _displayModule.drawEntity(_camera.worldPosToCameraPos(pos.value()), drawable.value());
        }
    }
}
