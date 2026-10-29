#pragma once
#include "Entity.hpp"
#include <optional>
#include <vector>

namespace fengine::ecs {
    class EntityManager {
        public:
            EntityManager();
            ~EntityManager();

            void killEntity(Entity const &e);

            Entity spawnEntity();

            Entity createEntity(std::size_t id);

            std::optional<Entity> entityFromIndex(std::size_t idx) const;
            std::optional<Entity> &entityFromIndex(std::size_t idx);

            std::size_t size();

            std::vector<std::optional<Entity>> &getEntities();

        private:
            std::vector<std::optional<Entity>> _entities;
    };
}
