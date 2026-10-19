#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/ComponentManager.hpp"
#include "f-engine/ecs/EntityManager.hpp"
#include "f-engine/ecs/SystemManager.hpp"

fengine::ecs::Registry::Registry(): systemManager(), entityManager(), componentManager() {}

fengine::ecs::Registry::~Registry() {}
