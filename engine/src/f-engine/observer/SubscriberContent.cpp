
#include "f-engine/ecs/observer/SubscriberContent.hpp"

fengine::ecs::observer::SubscriberContent::SubscriberContent(std::size_t entityId) : _entityId(entityId), _content() {}

fengine::ecs::observer::SubscriberContent::~SubscriberContent() {}

std::size_t fengine::ecs::observer::SubscriberContent::getEntityId() const {
    return _entityId;
}
