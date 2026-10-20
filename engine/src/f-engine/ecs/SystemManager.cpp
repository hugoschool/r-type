#include "f-engine/ecs/SystemManager.hpp"

fengine::ecs::SystemManager::SystemManager(): _systems(), _deltaTime(500), _clock(std::chrono::steady_clock::now()) {}

fengine::ecs::SystemManager::SystemManager(int customTime)
: _systems(), _deltaTime(customTime), _clock(std::chrono::steady_clock::now()) {}

fengine::ecs::SystemManager::~SystemManager() {}

void fengine::ecs::SystemManager::update(Registry &manager) {
    std::chrono::time_point<std::chrono::steady_clock> elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = elapsedTime - _clock;
    std::chrono::milliseconds var(_deltaTime);

    // TODO: replace the current time valuation system
    // if (time < var) {
    //     return;
    // }

    for (auto &[_, system] : _systems) {
        system->update(manager);
    }
    _clock = std::chrono::steady_clock::now();
}







