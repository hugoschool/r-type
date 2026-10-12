#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace fengine::ecs {
    class Registry {
        public:
            Registry() : systemManager(), entityManager(), componentManager() {};
            ~Registry() {};

            SystemManager systemManager;
            EntityManager entityManager;
            ComponentManager componentManager;

        private:
    };
}
