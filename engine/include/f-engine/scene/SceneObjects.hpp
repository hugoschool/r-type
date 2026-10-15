#pragma once

#include <string>

namespace fengine::scene {

    struct Button {
            float length;
            float height;
            float pos_x;
            float pos_y;
            std::string content;
    };

    struct Background {
            bool paralax;
            float offset;
            std::string texture;
    };
}
