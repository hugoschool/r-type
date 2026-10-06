#pragma once

#include "IEvent.hpp"
#include "EventType.hpp"
#include "EventKey.hpp"
#include "EventMouseButton.hpp"

namespace fengine {
    namespace modules {
        namespace display {
            class AEvent : public IEvent {
                protected:
                    EventKey _key;
                    EventType _type;
                    EventMouseButton _mouseButton;
                public:
                    AEvent(EventType type) : _type(type) {};
                    ~AEvent() {};

                    EventMouseButton getMouseButton() override {
                        return _mouseButton;
                    };
                    void setMouseButton(EventMouseButton mouseButton) override {
                        _mouseButton = mouseButton;
                    };

                    EventKey getKey() override {
                        return _key;
                    };
                    void setKey(EventKey key) override {
                        _key = key;
                    };

                    EventType getType() override {
                        return _type;
                    };
                    void setType(EventType type) override {
                        _type = type;
                    };
            };
        }
    }
}