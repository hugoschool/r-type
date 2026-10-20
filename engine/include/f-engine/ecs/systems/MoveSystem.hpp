#pragma once

#include "ISystem.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/systems/ASystem.hpp"

namespace fengine::ecs {
    class MovementSystem : public ASystem {
        public:
            MovementSystem(camera::Camera &camera);
            ~MovementSystem() override;

            void update(Registry &manager) override;

        private:
            camera::Camera &_camera;
    };
}
