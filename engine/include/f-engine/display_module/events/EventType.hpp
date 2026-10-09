#pragma once

namespace fengine::modules::display {
    enum class EventType {
        KeyPressed,
        KeyReleased,
        Click,

        // a part le quit peut etre inutile mais on sait jamais
        Reset,
        Quit,
        Menu
    };
}
