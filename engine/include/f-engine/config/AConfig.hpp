#pragma once

#include "f-engine/config/IConfig.hpp"
#include <map>

namespace fengine::config {
    class AConfig: public IConfig {
        public:
            AConfig();
            ~AConfig() override;

            void set(std::string, std::any) override;
            std::optional<std::any> get(std::string) override;

        protected:
            std::map<std::string, std::any> _configMap;
    };
}
