#pragma once

#include "AEvent.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"

namespace fengine::modules::display {
    class KeyReleasedEvent : public AEvent {
        public:
            KeyReleasedEvent(EventKey key);
            ~KeyReleasedEvent() override;
    };
}
