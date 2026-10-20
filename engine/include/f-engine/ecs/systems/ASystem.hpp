#pragma once

#include "ISystem.hpp"
#include "f-engine/ecs/observer/Subscriber.hpp"
#include "f-engine/ecs/observer/SubscriberContent.hpp"
#include <memory>
#include <vector>

namespace fengine::ecs {
    class ASystem : public ISystem {
        public:
            ASystem();
            ~ASystem() override;

            void addSubscriber(std::shared_ptr<observer::Subscriber> &sub);

            void updateSubscribers(observer::SubscriberContent &content);

        protected:
            std::vector<std::shared_ptr<observer::Subscriber>> _subscribers;
    };
}
