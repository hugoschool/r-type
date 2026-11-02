#pragma once

#include <string>

namespace fengine::scene {
    class Button {
        public:
            Button(float l, float h, float x, float y, std::string str, std::string ttsPath)
                : length(l), height(h), pos_x(x), pos_y(y), content(str),
                selected(false), tts(ttsPath) {};
            ~Button() {};

            float length;
            float height;
            float pos_x;
            float pos_y;
            std::string content;

            bool selected;

            std::string tts;

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
