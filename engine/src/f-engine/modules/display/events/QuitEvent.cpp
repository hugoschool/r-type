#include "f-engine/modules/display/events/QuitEvent.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::QuitEvent::QuitEvent()
    : fengine::modules::display::AEvent(fengine::modules::display::EventType::Quit) {
}

fengine::modules::display::QuitEvent::~QuitEvent() {
}
