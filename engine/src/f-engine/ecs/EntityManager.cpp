#include "f-engine/ecs/EntityManager.hpp"
#include <algorithm>

fengine::ecs::EntityManager::EntityManager() : _entities() {
}

fengine::ecs::EntityManager::~EntityManager() {
}

void fengine::ecs::EntityManager::killEntity(Entity const &e) {
    auto it = std::find(_entities.begin(), _entities.end(), std::make_optional<Entity>(e));

    if (it != _entities.end()) {
        (*it) = std::nullopt;
    }
};

fengine::ecs::Entity fengine::ecs::EntityManager::spawnEntity() {
    return createEntity(_entities.size());
};

fengine::ecs::Entity fengine::ecs::EntityManager::createEntity(std::size_t id) {
    std::optional<Entity> entity = std::make_optional<Entity>(Entity(id));
    _entities.push_back(entity);
    return entity.value();
};

std::optional<fengine::ecs::Entity> fengine::ecs::EntityManager::entityFromIndex(std::size_t idx) {
    if (!_entities[idx].has_value())
        return std::nullopt;
    return _entities[idx].value();
};

std::size_t fengine::ecs::EntityManager::size() {
    return _entities.size();
};

std::vector<std::optional<fengine::ecs::Entity>> &fengine::ecs::EntityManager::getEntities() {
    return _entities;
}
