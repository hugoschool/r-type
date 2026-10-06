#pragma once

#include "AEvent.hpp"
#include "EventMouseButton.hpp"

namespace fengine {
    namespace modules {
        namespace display {
            class ClickEvent : public AEvent {
                public:
                    ClickEvent(EventMouseButton mouseButton);
                    ~ClickEvent();
            };
        }
    }
}