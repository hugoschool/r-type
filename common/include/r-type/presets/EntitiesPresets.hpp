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

    struct AnimationValues {
            int length;
            int height;
            int offset_x;
            int offset_y;
            int cols;
            int rows;
    };

    struct presets {
            EntityType type;
            EntityPreset preset;
            AnimationValues values;
    };

    // There will also be an Animation Component preset
    constexpr presets EntityPresets[] = {
        {EntityType::PLAYERS, {"./assets/textures/ships.png", "NONE", "NONE"}, {33, 17, 0, 0, 5, 1}},
        {EntityType::BULLETS, {"./assets/textures/projectile.png", "./assets/sounds/ship_shot.ogg", "NONE"},
            {50, 18, 0, 0, 2, 1}},
        {EntityType::E_SHOOTER, {"./assets/textures/e_shooter.png", "NONE", "NONE"}, {33, 33, 0, 33, 3, 1}},
        {EntityType::E_BOSS, {"./assets/textures/e_boss.png", "NONE", "NONE"}, {264, 143, 0, 0, 2, 4}},
    };
}
