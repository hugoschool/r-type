#pragma once
#include "Entity.hpp"
#include "SparseArray.hpp"
#include <any>
#include <typeindex>
#include <unordered_map>

namespace fengine {
    namespace ecs {
        class ComponentManager {
            public:
                ComponentManager() {};
                ~ComponentManager() {};

                template <class Component>
                SparseArray<Component> &registerComponent() {
                    auto type = std::type_index(typeid(Component));
                    if (_componentsArrays.contains(type))
                        return std::any_cast<SparseArray<Component> &>(_componentsArrays.at(type));
                    SparseArray<Component> arr = SparseArray<Component>();

                    auto val = _componentsArrays.emplace(type, std::any_cast<SparseArray<Component>>(arr));
                    return std::any_cast<SparseArray<Component> &>((*val.first).second);
                };

                template <class Component>
                SparseArray<Component> &getComponents() {
                    auto type = std::type_index(typeid(Component));

                    if (_componentsArrays.contains(type))
                        return std::any_cast<SparseArray<Component> &>(_componentsArrays.at(type));
                    throw("Exception");
                };

                template <class Component>
                SparseArray<Component> const &getComponents() const {
                    auto type = std::type_index(typeid(Component));

                    if (_componentsArrays.contains(type))
                        return std::any_cast<SparseArray<Component> const &>(_componentsArrays.at(type));
                    throw("Exception");
                };

                template <typename Component>
                typename SparseArray<Component>::reference_type addComponent(Entity const &to, Component &&c) {
                    SparseArray<Component> &arr = getComponents<Component>();

                    return arr.insert_at(to.getId(), c);
                };

                template <typename Component, typename... Params>
                typename SparseArray<Component>::reference_type emplaceComponent(Entity const &to, Params &&...p) {
                    auto type = std::type_index(typeid(Component));

                    if (_componentsArrays.find(type) == _componentsArrays.end()) {
                        throw("Exception");
                    }

                    SparseArray<Component> &arr = std::any_cast<SparseArray<Component>>(_componentsArrays.at(type));
                    return arr.emplace_at(to.getId(), p...);
                };

                template <typename Component>
                void removeComponent(Entity const &from) {
                    auto type = std::type_index(typeid(Component));

                    if (_componentsArrays.find(type) == _componentsArrays.end()) {
                        return;
                    }

                    SparseArray<Component> &arr = std::any_cast<SparseArray<Component>>(_componentsArrays.at(type));
                    arr.erase(from.getId());
                };

            private:
                std::unordered_map<std::type_index, std::any> _componentsArrays;
        };
    }
}
