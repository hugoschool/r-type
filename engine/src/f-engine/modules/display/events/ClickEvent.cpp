#include "f-engine/modules/display/events/ClickEvent.hpp"
#include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::ClickEvent::ClickEvent(fengine::modules::display::EventMouseButton mouseButton)
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::Click) {
    _mouseButton = mouseButton;
}

fengine::modules::display::ClickEvent::~ClickEvent() {
}
