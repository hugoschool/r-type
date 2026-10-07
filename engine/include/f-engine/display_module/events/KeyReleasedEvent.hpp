#pragma once

#include "AEvent.hpp"
#include "f-engine/display_module/events/EventKey.hpp"

namespace fengine {
    namespace modules {
        namespace display {
            class KeyReleasedEvent : public AEvent {
                public:
                    KeyReleasedEvent(EventKey key);
                    ~KeyReleasedEvent();
            };
        }
    }
}