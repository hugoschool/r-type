#pragma once

#include "f-engine/display_module/shapes/IShape.hpp"
#include <memory>

namespace fengine {
    namespace modules {
        namespace display {
            class IWindow {
                public:
                    virtual ~IWindow() = default;

                    virtual void drawShape(std::shared_ptr<IShape>) = 0;
            };
        }
    }
}