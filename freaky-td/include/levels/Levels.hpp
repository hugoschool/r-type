#pragma once

#include "Level.hpp"
#include <string>
#include <vector>

namespace freakytd {
    class Levels {
        public:
            Levels();
            ~Levels() = default;

            // Returns an exception if couldn't parse
            void parseFile(std::string filename);

            std::vector<Level> &getLevels();
            std::optional<std::reference_wrapper<Level>> getCurrentLevel();
            void nextLevel();

        private:
            std::vector<Level> _levels;
            std::size_t _index;
    };
}
