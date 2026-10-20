#pragma once

#include "f-engine/ecs/components/PositionComponent.hpp"

namespace fengine::camera {
    class Camera {
        public:
            Camera(float baseVelocity, float maxLength);
            ~Camera();

            ecs::PositionComponent worldPosToCameraPos(ecs::PositionComponent &pos);

            void incrementWorldIndex();

            float getWorldIndex();

            float getBaseVelocity();

            float getMaxLength();

        private:
            float _currentWorldIndex;
            float _baseVelocity;
            float _maxLength;
    };
}
