#pragma once

#include "ISystem.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"

namespace fengine::ecs {
    class AnimationSystem : public ISystem {
        public:
            AnimationSystem(modules::display::IDisplayModule &displayModule, camera::Camera &camera);
            ~AnimationSystem() override;

            void update(Registry &manager) override;

        private:
            modules::display::IDisplayModule &_displayModule;
            camera::Camera &_camera;
    };
}
