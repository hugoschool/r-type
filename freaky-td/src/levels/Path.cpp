#include "levels/Path.hpp"
#include "Exception.hpp"
#include "Tile.hpp"
#include <iostream>

freakytd::Path::Path() : _tiles() {
}

void freakytd::Path::parse(const boost::json::value &value) {
    const auto &array = value.as_array();

    for (auto it = array.begin(); it < array.end(); it++) {
        if (!(*it).is_object()) {
            throw LevelException("Tile (inside of a path) is not an object");
        }

        const auto &obj = (*it).as_object();

        if (!obj.contains("x") || !obj.contains("y")) {
            throw LevelException("Tile isn't composed of x and y coordinates");
        }

        _tiles.push_back({obj.at("x").as_int64(), obj.at("y").as_int64()});
    }
}

std::vector<freakytd::Tile> &freakytd::Path::getTiles() {
    return _tiles;
}

freakytd::Tile &freakytd::Path::getStart() {
    return _tiles[0];
}

freakytd::Tile &freakytd::Path::getEnd() {
    return _tiles[_tiles.size() - 1];
}
