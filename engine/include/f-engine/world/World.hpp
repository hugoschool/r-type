#pragma once

#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Registry.hpp"

namespace fengine::world {
    class World {
        public:
            World(float baseVelocity, float maxLength);
            ~World();

            camera::Camera _camera;
            ecs::Registry _registry;
    };
}
