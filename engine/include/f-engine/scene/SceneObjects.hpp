#pragma once

#include <string>

namespace fengine::scene {
    class Button {
        public:
            Button(float l, float h, float x, float y, std::string str)
                : length(l), height(h), pos_x(x), pos_y(y), content(str) {};
            ~Button() {};

            float length;
            float height;
            float pos_x;
            float pos_y;
            std::string content;

            bool isInBounds(float x, float y) {
                return (pos_x < x + 1 && pos_x + length > x && pos_y < y + 1 && pos_y + height > y);
            };
    };

    struct Background {
            bool paralax;
            float offset;
            std::string texture;
    };
}
