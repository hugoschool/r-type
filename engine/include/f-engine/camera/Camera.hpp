#pragma once

#include "f-engine/ecs/components/PositionComponent.hpp"

namespace fengine::camera {
    class Camera {
        public:
            Camera(float baseVelocity, float maxLength)
                : _currentWorldIndex(0), _baseVelocity(baseVelocity), _maxLength(maxLength) {};
            ~Camera() {};

            ecs::PositionComponent worldPosToCameraPos(ecs::PositionComponent &pos) {
                ecs::PositionComponent newPos = {pos.pos_x - _currentWorldIndex, pos.pos_y, pos.can_be_out_window};
                return newPos;
            };

            void incrementWorldIndex() {
                _currentWorldIndex += _baseVelocity;
            };

            float getWorldIndex() {
                return _currentWorldIndex;
            };

            float getBaseVelocity() {
                return _baseVelocity;
            };

            float getMaxLength() {
                return _maxLength;
            };

        private:
            float _currentWorldIndex;
            float _baseVelocity;
            float _maxLength;
    };
}
