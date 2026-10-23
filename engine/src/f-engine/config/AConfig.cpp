#include "f-engine/config/AConfig.hpp"
#include <exception>
#include <optional>

fengine::config::AConfig::AConfig() : _configMap(), _changed(false) {
}

fengine::config::AConfig::~AConfig() {
}

void fengine::config::AConfig::set(std::string key, std::any value) {
    _configMap.insert_or_assign(key, value);
    _changed = true;
}

std::optional<std::any> fengine::config::AConfig::get(std::string key) {
    try {
        std::any value = _configMap.at(key);
        return value;
    } catch (std::exception) { return std::nullopt; }
}

bool fengine::config::AConfig::hasChanged() {
    return _changed;
}

void fengine::config::AConfig::resetChange() {
    _changed = false;
}
