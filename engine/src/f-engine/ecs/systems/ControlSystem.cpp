#include "f-engine/ecs/systems/ControlSystem.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"

fengine::ecs::ControlSystem::ControlSystem(std::queue<modules::display::EventKey> &pqueue,
    std::queue<modules::display::EventKey> &rqueue)
    : _pressedQueue(pqueue), _releasedQueue(rqueue) {};;

fengine::ecs::ControlSystem::~ControlSystem() {};

void fengine::ecs::ControlSystem::update(Registry &manager) {

    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
    auto &controllables = manager.componentManager.getComponents<ControllableComponent>();

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &vel = velocities[i];
        auto &control = controllables[i];

        if (vel.has_value() && control.has_value()) {
            if (!_pressedQueue.empty()) {
                modules::display::EventKey &key = _pressedQueue.front();
                if (key == modules::display::EventKey::Z)
                    vel->vel_y = -10;
                if (key == modules::display::EventKey::Q)
                    vel->vel_x = -10;
                if (key == modules::display::EventKey::S)
                    vel->vel_y = 10;
                if (key == modules::display::EventKey::D)
                    vel->vel_x = 10;
                _pressedQueue.pop();
            }
            if (!_releasedQueue.empty()) {
                modules::display::EventKey &key = _releasedQueue.front();
                if (key == modules::display::EventKey::Z)
                    vel->vel_y = 0;
                if (key == modules::display::EventKey::Q)
                    vel->vel_x = 0;
                if (key == modules::display::EventKey::S)
                    vel->vel_y = 0;
                if (key == modules::display::EventKey::D)
                    vel->vel_x = 0;
                _releasedQueue.pop();
            }
        }
    }
}
