#include "Tile.hpp"

freakytd::Tile::Tile(std::int64_t x, std::int64_t y) : _x(x), _y(y) {
}

std::int64_t freakytd::Tile::getX() const {
    return _x;
}

std::int64_t freakytd::Tile::getY() const {
    return _y;
}
