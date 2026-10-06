#pragma once

#include "AEvent.hpp"

namespace fengine {
    namespace modules {
        namespace display {
            class QuitEvent : public AEvent {
                public:
                    QuitEvent();
                    ~QuitEvent();
            };
        }
    }
}