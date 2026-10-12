#include "f-engine/modules/display/events/KeyReleasedEvent.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::KeyReleasedEvent::KeyReleasedEvent(fengine::modules::display::EventKey key)
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::KeyReleased) {
    _key = key;
}

fengine::modules::display::KeyReleasedEvent::~KeyReleasedEvent() {
}
