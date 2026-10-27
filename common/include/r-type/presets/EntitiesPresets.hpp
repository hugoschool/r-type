#pragma once

#include <string_view>

namespace fengine::presets {
    enum class EntityType {
        PLAYERS,
        BULLETS,
        E_SHOOTER,
        E_BOSS,
    };

    struct EntityPreset {
            std::string_view texture;
            std::string_view apparition_sound;
            std::string_view behaviour_sound;
    };

    struct presets {
            EntityType type;
            EntityPreset preset;
    };

    // There will also be an Animation Component preset
    constexpr presets EntityPresets[] = {
        {EntityType::PLAYERS, {"", "", ""}},
        {EntityType::BULLETS, {"", "./assets/sounds/ship_shot.ogg", ""}},
        {EntityType::E_SHOOTER, {"", "", ""}},
        {EntityType::E_BOSS, {"", "", ""}},
    };
}
