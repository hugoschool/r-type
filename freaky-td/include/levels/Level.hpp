#pragma once

#include "Path.hpp"
#include <boost/json.hpp>
#include <chrono>
#include <cstdint>
#include <set>
#include <string>

namespace freakytd {
    class Level {
        public:
            Level();
            ~Level() = default;

            // Can return an exception
            void parse(const boost::json::value &value);

            std::vector<Path> &getPaths();

            std::int64_t getEntityTotalAmount() const;
            std::chrono::seconds getEntitySpawnRate() const;
            std::set<std::string> &getEntityTypes();

            std::int64_t getPlayerHP() const;

        private:
            std::vector<Path> _paths;

            std::int64_t _entityTotalAmount;
            std::chrono::seconds _entitySpawnRate;
            // TODO: change to an enum whenever the enum is created
            std::set<std::string> _entityTypes;

            std::int64_t _playerHP;

            void parsePaths(const boost::json::object &value);
            void parseEntities(const boost::json::object &value);
            void parsePlayer(const boost::json::object &value);
    };
}
