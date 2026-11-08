#pragma once

#include <vector>
#include "levels/Path.hpp"

namespace freakytd {
    class IMapBuilder {
        public:
            virtual ~IMapBuilder() = default;

            virtual void generateTiles(std::vector<Path> &) = 0;
    };
}
