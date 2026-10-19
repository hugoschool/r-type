#include "f-engine/ecs/observer/Subscriber.hpp"
#include "f-engine/ecs/observer/SubscriberContent.hpp"

fengine::ecs::observer::Subscriber::Subscriber() {}

fengine::ecs::observer::Subscriber::~Subscriber() {}

void fengine::ecs::observer::Subscriber::send(fengine::ecs::observer::SubscriberContent &content) {
    _queue.push(content);
}

bool fengine::ecs::observer::Subscriber::empty() const {
    return _queue.empty();
}

fengine::ecs::observer::SubscriberContent fengine::ecs::observer::Subscriber::getContent() {
    SubscriberContent content = _queue.front();
    _queue.pop();
    return content;
}
