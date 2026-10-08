#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/display_module/IDisplayModule.hpp"

namespace fengine::ecs {
    class DisplaySystem : public ISystem {
        public:
            DisplaySystem(modules::display::IDisplayModule &displayModule): _displayModule(displayModule) {};
            ~DisplaySystem() override {};

            void update(Registry &manager) override {
                auto &positions = manager.componentManager.getComponents<PositionComponent>();
                auto &drawables = manager.componentManager.getComponents<DrawableComponent>();

                for (std::size_t i = 0; i < positions.size(); i++) {
                    auto &pos = positions[i];
                    auto &drawable = drawables[i];

                    if (pos.has_value() && drawable.has_value()) {
                        _displayModule.drawEntity(pos.value(), drawable.value());
                    }
                }
            };

        private:
            modules::display::IDisplayModule &_displayModule;
    };
}
