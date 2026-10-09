#pragma once
#include "Entity.hpp"
#include <algorithm>
#include <optional>
#include <vector>

namespace fengine::ecs {
    class EntityManager {
        public:
            EntityManager() : _entities() {};
            ~EntityManager() {};

            void killEntity(Entity const &e) {
                auto it = std::find(_entities.begin(), _entities.end(), std::make_optional<Entity>(e));

                if (it != _entities.end()) {
                    (*it) = std::nullopt;
                }
            };

            Entity spawnEntity() {
                std::optional<Entity> entity = std::make_optional<Entity>(Entity(_entities.size()));
                _entities.push_back(entity);
                return entity.value();
            };

            std::optional<Entity> entityFromIndex(std::size_t idx) {
                if (!_entities[idx].has_value())
                    return std::nullopt;
                return _entities[idx].value();
            };

            std::size_t size() {
                return _entities.size();
            };

        private:
            std::vector<std::optional<Entity>> _entities;
    };
}
