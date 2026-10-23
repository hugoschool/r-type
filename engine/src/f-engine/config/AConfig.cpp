#include "f-engine/config/AConfig.hpp"
#include <exception>
#include <optional>

fengine::config::AConfig::AConfig() : _configMap() {
}

fengine::config::AConfig::~AConfig() {
}

void fengine::config::AConfig::set(std::string key, std::any value) {
    _configMap.insert({key, value});
}

std::optional<std::any> fengine::config::AConfig::get(std::string key) {
    try {
        std::any value = _configMap.at(key);
        return value;
    } catch (std::exception) { return std::nullopt; }
}
