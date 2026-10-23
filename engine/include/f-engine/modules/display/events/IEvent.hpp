#pragma once
#include "EventKey.hpp"
#include "EventMouseButton.hpp"
#include "EventType.hpp"
#include <utility>

namespace fengine::modules::display {
    class IEvent {
        public:
            virtual ~IEvent() = default;

            virtual EventMouseButton getMouseButton() = 0;
            virtual void setMouseButton(EventMouseButton mouseButton) = 0;

            virtual EventKey getKey() = 0;
            virtual void setKey(EventKey key) = 0;

            virtual EventType getType() = 0;
            virtual void setType(EventType type) = 0;

            virtual std::pair<float, float> getClickCoordinates() = 0;
            virtual void setClickCoordinates(float x, float y) = 0;
    };
}
