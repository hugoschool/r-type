#pragma once

#include "AEvent.hpp"
#include "f-engine/display_module/events/EventKey.hpp"

namespace fengine::modules::display {
    class KeyReleasedEvent : public AEvent {
        public:
            KeyReleasedEvent(EventKey key);
            ~KeyReleasedEvent() override;
    };
}
