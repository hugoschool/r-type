#pragma once

#include "Tile.hpp"
#include <boost/json.hpp>
#include <vector>

namespace freakytd {
    class Path {
        public:
            Path();
            ~Path() = default;

            void parse(const boost::json::value &value);

            std::vector<Tile> &getTiles();
            Tile &getStart();
            Tile &getEnd();

        private:
            std::vector<Tile> _tiles;
    };
}
