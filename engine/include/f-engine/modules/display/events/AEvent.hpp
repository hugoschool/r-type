#pragma once

#include "EventKey.hpp"
#include "EventMouseButton.hpp"
#include "EventType.hpp"
#include "IEvent.hpp"

namespace fengine::modules::display {
    class AEvent : public IEvent {
        protected:
            EventKey _key;
            EventType _type;
            EventMouseButton _mouseButton;

        public:
            AEvent(EventType type);
            ~AEvent() override = default;

            EventMouseButton getMouseButton() override;
            void setMouseButton(EventMouseButton mouseButton) override;

            EventKey getKey() override;
            void setKey(EventKey key) override;

            EventType getType() override;
            void setType(EventType type) override;
    };
}
