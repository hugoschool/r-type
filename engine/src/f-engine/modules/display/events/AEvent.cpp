#include "f-engine/modules/display/events/AEvent.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventType.hpp"

fengine::modules::display::AEvent::AEvent(EventType type) : _type(type) {};

fengine::modules::display::EventMouseButton fengine::modules::display::AEvent::getMouseButton() {
    return _mouseButton;
};
void fengine::modules::display::AEvent::setMouseButton(EventMouseButton mouseButton) {
    _mouseButton = mouseButton;
};

fengine::modules::display::EventKey fengine::modules::display::AEvent::getKey() {
    return _key;
};
void fengine::modules::display::AEvent::setKey(EventKey key) {
    _key = key;
};

fengine::modules::display::EventType fengine::modules::display::AEvent::getType() {
    return _type;
};
void fengine::modules::display::AEvent::setType(EventType type) {
    _type = type;
};

std::pair<float, float> &fengine::modules::display::AEvent::getClickCoordinates() {
    return _clickCoords;
};
void fengine::modules::display::AEvent::setClickCoordinates(float x, float y) {
    _clickCoords.first = x;
    _clickCoords.second = y;
};
