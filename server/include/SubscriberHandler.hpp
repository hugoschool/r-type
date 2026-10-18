#pragma once

#include "ClientHandler.hpp"
#include "f-engine/ecs/observer/SubscriberContent.hpp"
#include <f-engine/ecs/Registry.hpp>
#include <f-engine/ecs/observer/Subscriber.hpp>
#include <functional>

namespace rtype::server {
    class SubscriberHandler {
        public:
            SubscriberHandler(ClientHandler &clientHandler);
            ~SubscriberHandler() = default;

            void add(fengine::ecs::Registry &registry);
            void process();

        private:
            using shared_subscriber = std::shared_ptr<fengine::ecs::observer::Subscriber>;
            ClientHandler &_clientHandler;

            std::vector<std::pair<shared_subscriber, std::function<void(shared_subscriber &subscriber)>>> _subscribers;

            void processMovement(shared_subscriber &subscriber);
            void processBoundary(shared_subscriber &subscriber);

            void handlePositionComponent(fengine::ecs::observer::SubscriberContent &content);
            void handleVelocityComponent(fengine::ecs::observer::SubscriberContent &content);
            void handleDeadComponent(fengine::ecs::observer::SubscriberContent &content);
    };
}
