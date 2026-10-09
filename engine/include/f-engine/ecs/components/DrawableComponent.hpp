#pragma once

#include "f-engine/display_module/shapes/IShape.hpp"
#include <memory>
#include <optional>
#include <string>
#include <tuple>

namespace fengine::ecs {
    struct DrawableComponent {
            std::optional<std::string> texture_filepath;
            std::shared_ptr<modules::display::IShape> shape;
            std::optional<std::tuple<int, int, int>> color;
    };
}
