#pragma once

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace fengine {
    namespace ecs {
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
}
