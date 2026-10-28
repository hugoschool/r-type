#pragma once

#include "f-engine/config/IConfig.hpp"
#include "f-engine/modules/display/shapes/IShape.hpp"
#include <memory>

constexpr int WINDOW_X = 1000;
constexpr int WINDOW_Y = 800;

namespace fengine::modules::display {
    class IWindow {
        public:
            virtual ~IWindow() = default;

            virtual void drawShape(std::shared_ptr<IShape>) = 0;
            virtual void changeConfig(std::shared_ptr<config::IConfig>) = 0;
    };
}
