#pragma once

#include "systems/ISystem.hpp"
#include <chrono>
#include <concepts>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace fengine::ecs {
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

            template <is_system T, typename... Args>
            void addSystem(Args &&...args) {
                std::shared_ptr<T> system = std::make_shared<T>(std::forward<Args>(args)...);

                _systems.insert({typeid(T), std::move(system)});
            };

            template <is_system T>
            T *getSystem() {
                try {
                    auto system = _systems.at(typeid(T));
                    return dynamic_cast<T *>(system.get());
                } catch (std::exception &) {
                    return nullptr;
                }
            };

            template <is_system T>
            void removeSystem() {
                _systems.erase(typeid(T));
            };

            void update(Registry &manager) {
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
            };

        private:
            std::unordered_map<std::type_index, std::shared_ptr<ISystem>> _systems;
            int _deltaTime;
            std::chrono::time_point<std::chrono::steady_clock> _clock;
    };
}
