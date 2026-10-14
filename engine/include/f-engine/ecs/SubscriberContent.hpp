#pragma once

#include <any>
#include <map>
#include <typeindex>

namespace fengine::ecs::observer {
    class SubscriberContent {
        public:
            SubscriberContent() : _content() {};
            ~SubscriberContent() = default;

            template <class T>
            void add(T &component) {
                _content.insert({typeid(T), component});
            }

            template <class T>
            T retrieve() {
                return _content[typeid(T)];
            }

        private:
            std::map<std::type_index, std::any> _content;
    };
}
