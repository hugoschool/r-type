#pragma once

#include "f-engine/display_module/shapes/IShape.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace fengine {
    namespace modules {
        namespace display {
            class SFMLCircle: public IShape {
                public:
                    SFMLCircle(float radius) {
                        shape = sf::CircleShape();

                        shape.setRadius(radius);
                    };
                    ~SFMLCircle() {};

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

                    sf::CircleShape shape;
            };
        }
    }
}
