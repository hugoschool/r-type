#pragma once

namespace freakytd {
    enum class TileType {
        BASIC,
        PATH,
        TOWER,
    };

    struct TileComponent {
        TileType type;
    };
}
