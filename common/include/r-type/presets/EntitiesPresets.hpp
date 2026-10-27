#pragma once

namespace fengine::presets {

    enum class entity_types {
        PLAYERS,
        BULLETS,
        E_SHOOTER,
        E_BOSS,
    };

    struct entity_preset {
            char const *texture;
            char const *apparition_sound;
            char const *behaviour_sound;
    };

    struct presets {
            entity_types type;
            entity_preset preset;
    };

    // There will also be an Animation Component preset
    constexpr presets EntityPresets[] = {
        {entity_types::PLAYERS, {"", "", ""}},
        {entity_types::BULLETS, {"", "./assets/sounds/ship_shot.ogg", ""}},
        {entity_types::E_SHOOTER, {"", "", ""}},
        {entity_types::E_BOSS, {"", "", ""}},
    };
}
