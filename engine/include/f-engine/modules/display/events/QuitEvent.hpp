#pragma once

#include "AEvent.hpp"

namespace fengine::modules::display {
    class QuitEvent : public AEvent {
        public:
            QuitEvent();
            ~QuitEvent() override;
    };
}
