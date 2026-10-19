#pragma once

#include "ISystem.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"

namespace fengine::ecs {
    class DisplaySystem : public ASystem {
        public:
            DisplaySystem(modules::display::IDisplayModule &displayModule, camera::Camera &camera);
            ~DisplaySystem() override;

            void update(Registry &manager) override;

        private:
            modules::display::IDisplayModule &_displayModule;
            camera::Camera &_camera;
    };
}
