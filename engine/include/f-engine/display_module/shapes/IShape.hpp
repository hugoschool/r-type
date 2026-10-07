#pragma once

#include "f-engine/ecs/components/PositionComponent.hpp"
#include <any>
#include <tuple>

namespace fengine {
    namespace modules {
        namespace display {
            class IShape {
                public:
                    virtual ~IShape() = default;

                    virtual void setPosition(ecs::PositionComponent &) = 0;
                    virtual void setColor(std::tuple<int, int, int> &) = 0;

                    virtual void setTexture(std::any) = 0;
            };
        }
    }
}
