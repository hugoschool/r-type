#include "f-engine/world/World.hpp"

fengine::world::World::World(float baseVelocity, float maxLength) : _camera(baseVelocity, maxLength), _registry() {}

fengine::world::World::~World() {}
