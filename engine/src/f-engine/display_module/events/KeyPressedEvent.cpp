#include "f-engine/display_module/events/KeyPressedEvent.hpp"
#include "f-engine/display_module/events/EventKey.hpp"
#include "f-engine/display_module/events/EventType.hpp"

fengine::modules::display::KeyPressedEvent::KeyPressedEvent(fengine::modules::display::EventKey key)
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::KeyPressed) {
    _key = key;
}

fengine::modules::display::KeyPressedEvent::~KeyPressedEvent() {
}
