#include "f-engine/ecs/systems/MoveSystem.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"

fengine::ecs::MovementSystem::MovementSystem(camera::Camera &camera) : _camera(camera) {};

fengine::ecs::MovementSystem::~MovementSystem() {};

void fengine::ecs::MovementSystem::update(Registry &manager) {
    auto &positions = manager.componentManager.getComponents<PositionComponent>();
    auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
    auto &boundaries = manager.componentManager.getComponents<BoundaryComponent>();
    // float baseVel = _camera.getBaseVelocity();
    // float maxLength = _camera.getMaxLength();
    float worldIndex = _camera.getWorldIndex();
    BoundaryComponent boundary = {0, 0, false, false, std::nullopt};

    for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
        if (!manager.entityManager.entityFromIndex(i).has_value())
            continue;
        auto &pos = positions[i];
        auto &vel = velocities[i];
        auto &bound = boundaries[i];

        if (!pos.has_value())
            continue;

        // if (pos.value().pos_x + baseVel < maxLength) // enlever les commentaires quand il y aura le handling de la camera avec un camera.incrementworldindex qqpart
        //     pos.value().pos_x += baseVel;

        if (!vel.has_value())
            continue;

        if (bound.has_value()) {
            boundary = {bound.value().size_x, bound.value().size_y, bound.value().bouncy, bound.value().hittable,
                bound.value().can_be_hit};
        } else {
            boundary = {0, 0, false, false, std::nullopt};
        }

        if (!pos.value().can_be_out_window) {
            // trouver un moten de recup de l
            if (pos.value().pos_x > WINDOW_X - boundary.size_x + worldIndex) {
                pos.value().pos_x = WINDOW_X - boundary.size_x + worldIndex;
                vel.value().vel_x = -vel.value().vel_x;
            }
            if (pos.value().pos_x < 0 + worldIndex) {
                pos.value().pos_x = 0 + worldIndex;
                vel.value().vel_x = -vel.value().vel_x;
            }
            if (pos.value().pos_y > WINDOW_Y - boundary.size_y) {
                pos.value().pos_y = WINDOW_Y - boundary.size_y;
                vel.value().vel_y = -vel.value().vel_y;
            }
            if (pos.value().pos_y < 0) {
                pos.value().pos_y = 0;
                vel.value().vel_y = -vel.value().vel_y;
            }
        }

        observer::SubscriberContent content(i);

        if (pos.value().can_be_out_window) {
            // handle this differently so that entities can come from any corner of the window without getting slimed out instantly
            if (pos.value().pos_x > WINDOW_X + worldIndex || pos.value().pos_y > WINDOW_Y
                || pos.value().pos_x < 0 + worldIndex || pos.value().pos_y < 0) {
                std::optional<Entity> entity = manager.entityManager.entityFromIndex(i);
                if (entity.has_value()) {
                    DeadComponent deadComp = {entity.value().getId()};
                    manager.entityManager.killEntity(entity.value());
                    content.add<DeadComponent>(deadComp);
                }
            }
        }

        pos.value().pos_x += vel.value().vel_x;
        pos.value().pos_y += vel.value().vel_y;

        content.add<PositionComponent>(pos.value());
        content.add<VelocityComponent>(vel.value());
        updateSubscribers(content);
    }
}
