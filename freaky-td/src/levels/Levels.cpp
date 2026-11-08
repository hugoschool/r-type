#include "levels/Levels.hpp"
#include "Exception.hpp"
#include "levels/Level.hpp"
#include <boost/json.hpp>
#include <fstream>
#include <sstream>

freakytd::Levels::Levels() : _levels(), _index(0) {};

void freakytd::Levels::parseFile(std::string filename) {
    boost::json::stream_parser p;
    boost::system::error_code ec;

    std::ifstream f(filename);
    std::stringstream buffer;
    buffer << f.rdbuf();
    p.write(buffer.str(), ec);

    if (ec) {
        throw LevelException("Boost error code: " + ec.to_string());
    }
    p.finish(ec);
    if (ec) {
        throw LevelException("Boost error code: " + ec.to_string());
    }

    boost::json::value value = p.release();
    if (!value.is_array()) {
        throw LevelException("Levels file is not an array");
    }

    auto const &arr = value.get_array();
    if (!arr.empty()) {
        for (auto it = arr.begin(); it < arr.end(); it++) {
            if (!(*it).is_object()) {
                throw LevelException("Level is not an object");
            }

            Level level;
            level.parse(*it);

            _levels.emplace_back(std::move(level));
        }
    }
}

std::vector<freakytd::Level> &freakytd::Levels::getLevels() {
    return _levels;
}

std::optional<std::reference_wrapper<freakytd::Level>> freakytd::Levels::getCurrentLevel() {
    if (_levels.size() == 0) {
        return std::nullopt;
    }
    return _levels[_index];
}

void freakytd::Levels::nextLevel() {
    if (_index < _levels.size() - 1)
        _index += 1;
    else
        _index = 0;
}
