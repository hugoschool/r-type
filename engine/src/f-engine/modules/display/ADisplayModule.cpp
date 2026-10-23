#include "f-engine/modules/display/ADisplayModule.hpp"
#include "f-engine/config/IConfig.hpp"

fengine::modules::display::ADisplayModule::ADisplayModule() : _config() {
}

fengine::modules::display::ADisplayModule::~ADisplayModule() {
}

void fengine::modules::display::ADisplayModule::setConfig(std::shared_ptr<fengine::config::IConfig> config) {
    _config = config;
}
