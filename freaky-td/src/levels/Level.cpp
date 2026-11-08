#include "levels/Level.hpp"
#include "Exception.hpp"
#include "levels/Path.hpp"
#include <boost/json.hpp>

freakytd::Level::Level() : _paths(), _entitySpawnRate(), _entityTypes(), _playerHP(0) {};

void freakytd::Level::parse(const boost::json::value &value) {
    const auto &obj = value.as_object();

    parsePaths(obj);
    parseEntities(obj);
    parsePlayer(obj);
}

void freakytd::Level::parsePaths(const boost::json::object &obj) {
    if (!obj.contains("paths")) {
        throw LevelException("Level doesn't contain paths");
    }

    const auto &pathsValue = obj.at("paths");

    if (!pathsValue.is_array()) {
        throw LevelException("Paths is not an array");
    }

    const auto &paths = pathsValue.as_array();

    for (auto it = paths.begin(); it < paths.end(); it++) {
        if (!(*it).is_array()) {
            throw LevelException("Path is not composed of an array");
        }

        Path path;

        path.parse(*it);
        _paths.emplace_back(std::move(path));
    }
}

void freakytd::Level::parseEntities(const boost::json::object &obj) {
    if (!obj.contains("entities")) {
        throw LevelException("Level doesn't contain entity");
    }

    const auto &entityValue = obj.at("entities");

    if (!entityValue.is_object()) {
        throw LevelException("Entity is not an object");
    }

    const auto &entity = entityValue.as_object();

    if (!entity.contains("spawn_rate") || !entity.contains("types") || !entity.contains("total_amount")) {
        throw LevelException("Entity has missing attributes");
    }

    const auto &totalAmount = entity.at("total_amount");

    if (!totalAmount.is_int64()) {
        throw LevelException("Entity total amount isn't a number");
    }

    _entityTotalAmount = totalAmount.as_int64();

    const auto &spawnRate = entity.at("spawn_rate");

    if (!spawnRate.is_int64()) {
        throw LevelException("Entity spawn rate isn't a number");
    }

    _entitySpawnRate = std::chrono::seconds(spawnRate.as_int64());

    if (!entity.at("types").is_array()) {
        throw LevelException("Entity types is not an array");
    }

    const auto &typesArray = entity.at("types").as_array();

    for (auto it = typesArray.begin(); it < typesArray.end(); it++) {
        if (!(*it).is_string()) {
            throw LevelException("Entity type is not a string");
        }

        std::string type = std::string((*it).as_string());
        _entityTypes.insert(type);
    }
}

void freakytd::Level::parsePlayer(const boost::json::object &obj) {
    if (!obj.contains("player")) {
        throw LevelException("Level doesn't contain player");
    }

    const auto &playerValue = obj.at("player");

    if (!playerValue.is_object()) {
        throw LevelException("Player is not an object");
    }

    const auto &player = playerValue.as_object();

    if (!player.contains("health_points")) {
        throw LevelException("Player has missing attributes");
    }

    if (!player.at("health_points").is_int64()) {
        throw LevelException("Player HP isn't a number");
    }

    _playerHP = player.at("health_points").as_int64();
}

std::vector<freakytd::Path> &freakytd::Level::getPaths() {
    return _paths;
}

std::int64_t freakytd::Level::getEntityTotalAmount() const {
    return _entityTotalAmount;
}

std::chrono::seconds freakytd::Level::getEntitySpawnRate() const {
    return _entitySpawnRate;
}

std::set<std::string> &freakytd::Level::getEntityTypes() {
    return _entityTypes;
}

std::int64_t freakytd::Level::getPlayerHP() const {
    return _playerHP;
}
