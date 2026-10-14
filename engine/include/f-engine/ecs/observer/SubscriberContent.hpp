#pragma once

#include <any>
#include <cstddef>
#include <map>
#include <typeindex>

namespace fengine::ecs::observer {
    class SubscriberContent {
        public:
            SubscriberContent(std::size_t entityId) : _entityId(entityId), _content() {};
            ~SubscriberContent() = default;

            template <class T>
            void add(T &component) {
                _content.insert({typeid(T), component});
            }

            template <class T>
            T retrieve() {
                return _content[typeid(T)];
            }

            std::size_t getEntityId() const {
                return _entityId;
            }

        private:
            std::size_t _entityId;
            std::map<std::type_index, std::any> _content;
    };
}
