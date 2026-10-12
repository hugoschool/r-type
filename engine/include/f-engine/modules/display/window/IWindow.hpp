#pragma once

#include "f-engine/modules/display/shapes/IShape.hpp"
#include <memory>

namespace fengine::modules::display {
    class IWindow {
        public:
            virtual ~IWindow() = default;

            virtual void drawShape(std::shared_ptr<IShape>) = 0;
    };
}
