#pragma once

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include "f-engine/display_module/shapes/IShape.hpp"

namespace fengine {
    namespace ecs {
        struct DrawableComponent {
            std::optional<std::string> texture_filepath;
            std::shared_ptr<modules::display::IShape> shape;
            std::optional<std::tuple<int, int, int>> color;
        };
    }
}
