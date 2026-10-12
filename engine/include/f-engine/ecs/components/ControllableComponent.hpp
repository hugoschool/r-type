#pragma once

#include "f-engine/modules/display/events/EventKey.hpp"

namespace fengine::ecs {
    struct ControllableComponent {
            modules::display::EventKey up;
            modules::display::EventKey left;
            modules::display::EventKey down;
            modules::display::EventKey right;
    };
}
