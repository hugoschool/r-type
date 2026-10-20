#include "f-engine/ecs/systems/ASystem.hpp"

fengine::ecs::ASystem::ASystem() {
}

fengine::ecs::ASystem::~ASystem() {
}

void fengine::ecs::ASystem::addSubscriber(std::shared_ptr<observer::Subscriber> &sub) {
    _subscribers.push_back(sub);
}

void fengine::ecs::ASystem::updateSubscribers(observer::SubscriberContent &content) {
    for (auto &sub : _subscribers) {
        sub->send(content);
    }
}
