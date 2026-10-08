#pragma once

#include "f-engine/display_module/shapes/IShape.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace fengine::modules::display {
    class SFMLRectangle: public IShape {
        public:
            SFMLRectangle(float width, float height) {
                shape = sf::RectangleShape();

                shape.setSize(sf::Vector2f(width, height));
            };

            ~SFMLRectangle() {};

            void setPosition(ecs::PositionComponent &pos) override {
                shape.setPosition(sf::Vector2f(pos.pos_x, pos.pos_y));
            };
            void setColor(std::tuple<int, int, int> &color) override {
                int a = 255;
                int r = std::get<0>(color);
                int g = std::get<1>(color);
                int b = std::get<2>(color);
                sf::Color col = sf::Color(r, g, b, a);
                shape.setFillColor(col);
            };
            void setTexture(std::any texture) override {
                shape.setTexture(std::any_cast<sf::Texture *>(texture));
            };

            sf::RectangleShape shape;
    };
}
