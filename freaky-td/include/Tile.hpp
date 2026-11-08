#pragma once

#include <cstdint>

namespace freakytd {
    class Tile {
        public:
            Tile(std::int64_t x, std::int64_t y);
            ~Tile() = default;

            std::int64_t getX() const;
            std::int64_t getY() const;

        private:
            std::int64_t _x;
            std::int64_t _y;
    };
}
