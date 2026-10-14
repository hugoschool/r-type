#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/observer/Subscriber.hpp"
#include "f-engine/ecs/observer/SubscriberContent.hpp"
#include <vector>

namespace fengine::ecs {
    class ASystem : public ISystem {
        public:
            ASystem() {};
            ~ASystem() override {};

            void addSubscriber(observer::Subscriber &sub) {
                _subscribers.push_back(sub);
            };

            void updateSubscribers(observer::SubscriberContent &content) {
                for (observer::Subscriber &sub : _subscribers) {
                    sub.send(content);
                }
            };

        protected:
            std::vector<observer::Subscriber> _subscribers;
    };
}
