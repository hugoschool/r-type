#pragma once

#include "ISystem.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"

namespace fengine::ecs {
    class DisplaySystem : public ISystem {
        public:
            DisplaySystem(modules::display::IDisplayModule &displayModule, camera::Camera &camera) : _displayModule(displayModule), _camera(camera) {};
            ~DisplaySystem() override {};

            void update(Registry &manager) override {
                _camera.incrementWorldIndex();
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
            };

        private:
            modules::display::IDisplayModule &_displayModule;
            camera::Camera &_camera;
    };
}
