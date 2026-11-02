#pragma once

#include "r-type/proto/packets.pb.h"
#include <string_view>

namespace fengine::presets {
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
            rtype::packets::EntityType type;
            EntityPreset preset;
            AnimationValues values;
    };

    // There will also be an Animation Component preset
    constexpr presets EntityPresets[] = {
        {rtype::packets::ENTITY_PLAYERS, {"./assets/textures/ships.png", "NONE", "NONE"}, {33, 17, 0, 0, 5, 1}},
        {rtype::packets::ENTITY_BULLETS, {"./assets/textures/projectile.png", "./assets/sounds/ship_shot.ogg", "NONE"},
            {50, 18, 0, 0, 2, 1}},
        {rtype::packets::ENTITY_SHOOTER, {"./assets/textures/e_shooter.png", "NONE", "NONE"}, {33, 33, 0, 33, 3, 1}},
        {rtype::packets::ENTITY_BOSS, {"./assets/textures/e_boss.png", "NONE", "NONE"}, {264, 143, 0, 0, 2, 4}},
    };
}
