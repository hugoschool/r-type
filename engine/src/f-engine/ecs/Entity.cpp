#include "f-engine/ecs/Entity.hpp"

fengine::ecs::Entity::Entity(std::size_t id) : _id(id) {
}

fengine::ecs::Entity::~Entity() {
}

fengine::ecs::Entity::operator std::size_t() {
    return _id;
}

std::size_t fengine::ecs::Entity::getId() const {
    return _id;
};

bool fengine::ecs::Entity::operator==(const Entity &other) const {
    return _id == other._id;
}
