#pragma once

#include "AEvent.hpp"
#include "f-engine/display_module/events/EventKey.hpp"

namespace fengine::modules::display {
    class KeyPressedEvent : public AEvent {
        public:
            KeyPressedEvent(EventKey key);
            ~KeyPressedEvent();
    };
}
