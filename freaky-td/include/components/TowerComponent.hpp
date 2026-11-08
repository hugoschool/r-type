#pragma once

#include <chrono>
#include <utility>

namespace freakytd {

    enum class TowerType {
        ARCHER,
        MAGICIAN,
    };

    enum class DamageType {
        PHYSICAL,
        MAGICAL,
    };

    struct TowerComponent {
        std::pair<float, float> damage;
        float range;
        DamageType damageType;
        TowerType towerType;
        int attackSpeed;

        std::chrono::time_point<std::chrono::steady_clock> timeBeforeAttack;
    };
}
