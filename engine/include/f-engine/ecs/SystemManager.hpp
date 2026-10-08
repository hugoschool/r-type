#pragma once

#include "systems/ISystem.hpp"
#include <chrono>
#include <concepts>
#include <memory>
#include <vector>

namespace fengine {
    namespace ecs {
        template <typename T>
        concept is_system = std::derived_from<T, ISystem> && requires(T v, Registry &manager) {
            { v.update(manager) } -> std::same_as<void>;
        };

        class SystemManager {
            public:
                SystemManager() : _systems(), _deltaTime(500), _clock(std::chrono::steady_clock::now()) {};
                SystemManager(int customTime)
                    : _systems(), _deltaTime(customTime), _clock(std::chrono::steady_clock::now()) {};
                ~SystemManager() {};

                template <is_system T>
                void addSystem() {
                    for (auto &system : _systems) {
                        if (dynamic_cast<T *>(system.get())) {
                            return;
                        }
                    }
                    std::unique_ptr<T> system = std::make_unique<T>();

                    _systems.push_back(std::move(system));
                };

                template <is_system T>
                T *getSystem() {
                    for (auto &system : _systems) {
                        if (dynamic_cast<T *>(system.get())) {
                            return system;
                        }
                    }
                    return nullptr;
                };

                template <is_system T>
                void removeSystem() {
                    for (auto it = _systems.begin(); it != _systems.end(); ++it) {
                        if (dynamic_cast<T *>(it->get())) {
                            _systems.erase(it);
                            return;
                        }
                    }
                };

                void update(Registry &manager) {
                    std::chrono::time_point<std::chrono::steady_clock> elapsedTime = std::chrono::steady_clock::now();
                    std::chrono::duration<double> time = elapsedTime - _clock;
                    std::chrono::milliseconds var(_deltaTime);

                    if (time < var) {
                        return;
                    }

                    for (auto &system : _systems) {
                        system->update(manager);
                    }
                    _clock = std::chrono::steady_clock::now();
                };

            private:
                std::vector<std::unique_ptr<ISystem>> _systems;
                int _deltaTime;
                std::chrono::time_point<std::chrono::steady_clock> _clock;
        };
    }
}
