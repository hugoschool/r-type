#pragma once

#include "AEvent.hpp"
#include "EventMouseButton.hpp"

namespace fengine::modules::display {
    class ClickEvent : public AEvent {
        public:
            ClickEvent(EventMouseButton mouseButton);
            ~ClickEvent() override;
    };
}
