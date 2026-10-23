#pragma once

#include <any>
#include <optional>
#include <string>

namespace fengine::config {
    class IConfig {
        public:
            virtual ~IConfig() = default;

            virtual void set(std::string, std::any) = 0;
            virtual std::optional<std::any> get(std::string) = 0;
    };
}
