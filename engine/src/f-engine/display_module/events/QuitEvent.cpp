#include "f-engine/display_module/events/QuitEvent.hpp"
#include "f-engine/display_module/events/EventType.hpp"

fengine::modules::display::QuitEvent::QuitEvent() : fengine::modules::display::AEvent(fengine::modules::display::EventType::Quit)
{
}

fengine::modules::display::QuitEvent::~QuitEvent()
{
}
