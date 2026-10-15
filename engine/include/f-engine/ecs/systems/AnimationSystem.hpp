#pragma once

#include "ISystem.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"

namespace fengine::ecs {
    class AnimationSystem : public ISystem {
        public:
            AnimationSystem(modules::display::IDisplayModule &displayModule, camera::Camera &camera) : _displayModule(displayModule), _camera(camera) {};
            ~AnimationSystem() override {};

            void update(Registry &manager) override {
                _camera.incrementWorldIndex();
                auto &velocities = manager.componentManager.getComponents<VelocityComponent>();
                auto &drawables = manager.componentManager.getComponents<DrawableComponent>();
                auto &animations = manager.componentManager.getComponents<AnimationComponent>();
                float baseVelocity = _camera.getBaseVelocity();

                for (std::size_t i = 0; i < manager.entityManager.size(); i++) {
                    if (!manager.entityManager.entityFromIndex(i).has_value())
                        continue;
                    auto &vel = velocities[i];
                    auto &drawable = drawables[i];
                    auto &anim = animations[i];

                    if (vel.has_value() && drawable.has_value() && anim.has_value()) {
                        std::chrono::time_point<std::chrono::steady_clock> elapsedTime =
                            std::chrono::steady_clock::now();
                        std::chrono::duration<double> movementTime = elapsedTime - anim.value().anim_clock;
                        std::chrono::milliseconds var(150);

                        if (movementTime < var)
                            continue;
                        if (vel.value().vel_x > baseVelocity || vel.value().vel_y > baseVelocity || vel.value().vel_x < -baseVelocity || vel.value().vel_y < -baseVelocity) {
                            if (anim.value().cols > 1)
                                anim.value().offset_x += anim.value().length;
                            if (anim.value().offset_x > anim.value().length * (anim.value().cols - 1)) {
                                if (anim.value().rows > 1)
                                    anim.value().offset_y += anim.value().height;
                                if (anim.value().offset_y > anim.value().height * (anim.value().rows - 1)) {
                                    anim.value().offset_y = 0;
                                }
                                anim.value().offset_x = 0;
                            }
                            _displayModule.animateEntity(anim.value(), drawable.value());
                        }
                        anim.value().anim_clock = std::chrono::steady_clock::now();
                    }
                }
            };

        private:
            modules::display::IDisplayModule &_displayModule;
            camera::Camera &_camera;
    };
}
