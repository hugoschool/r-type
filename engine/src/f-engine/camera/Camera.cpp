#include "f-engine/camera/Camera.hpp"

fengine::camera::Camera::Camera(float baseVelocity, float maxLength)
    : _currentWorldIndex(0), _baseVelocity(baseVelocity), _maxLength(maxLength) {};

fengine::camera::Camera::~Camera() {
}

fengine::ecs::PositionComponent fengine::camera::Camera::worldPosToCameraPos(ecs::PositionComponent &pos) {
    ecs::PositionComponent newPos = {pos.pos_x - _currentWorldIndex, pos.pos_y, pos.can_be_out_window};
    return newPos;
};

void fengine::camera::Camera::incrementWorldIndex() {
    _currentWorldIndex += _baseVelocity;
};

float fengine::camera::Camera::getWorldIndex() {
    return _currentWorldIndex;
};

float fengine::camera::Camera::getBaseVelocity() {
    return _baseVelocity;
};

float fengine::camera::Camera::getMaxLength() {
    return _maxLength;
};
