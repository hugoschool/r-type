#pragma once

#include "f-engine/config/IConfig.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include <memory>

namespace fengine::modules::display {
    class ADisplayModule: public IDisplayModule {
        public:
            ADisplayModule();
            ~ADisplayModule() override;

            void setConfig(std::shared_ptr<config::IConfig>) override;
        protected:
            std::shared_ptr<config::IConfig> _config;
    };
}
