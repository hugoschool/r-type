#pragma once

#include "SubscriberContent.hpp"
#include <queue>

namespace fengine::ecs::observer {
    class Subscriber {
        public:
            Subscriber();
            ~Subscriber();

            void send(SubscriberContent &content);

            bool empty() const;

            SubscriberContent getContent();

        private:
            std::queue<SubscriberContent> _queue;
    };
}
