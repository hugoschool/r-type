#pragma once

#include "SubscriberContent.hpp"
#include <queue>

namespace fengine::ecs::observer {
    class Subscriber {
        public:
            Subscriber() {};
            ~Subscriber() {};

            void send(SubscriberContent &content) {
                _queue.push(content);
            };

            bool empty() const {
                return _queue.empty();
            };

            SubscriberContent getContent() {
                SubscriberContent content = _queue.front();
                _queue.pop();
                return content;
            };

        private:
            std::queue<SubscriberContent> _queue;
    };
}
