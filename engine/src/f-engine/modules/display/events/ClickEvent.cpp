#include "f-engine/modules/display/events/ClickEvent.hpp"
#include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::ClickEvent::ClickEvent(fengine::modules::display::EventMouseButton mouseButton, float x, float y)
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::Click) {
    _mouseButton = mouseButton;
    _clickCoords.first = x;
    _clickCoords.second = y;
}

fengine::modules::display::ClickEvent::~ClickEvent() {
}
