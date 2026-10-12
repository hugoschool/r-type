#include "f-engine/modules/display/events/KeyPressedEvent.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::KeyPressedEvent::KeyPressedEvent(fengine::modules::display::EventKey key)
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::KeyPressed) {
    _key = key;
}

fengine::modules::display::KeyPressedEvent::~KeyPressedEvent() {
}
