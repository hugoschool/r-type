#pragma once

#include "f-engine/config/AConfig.hpp"

namespace fengine::config {
    class ClientConfig : public AConfig {
        public:
            ClientConfig() : AConfig() {};
            ~ClientConfig() override {};
    };
}
