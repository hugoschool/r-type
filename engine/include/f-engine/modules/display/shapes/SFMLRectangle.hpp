#pragma once

#include "f-engine/modules/display/shapes/IShape.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace fengine::modules::display {
    class SFMLRectangle : public IShape {
        public:
            SFMLRectangle(float width, float height);
            ~SFMLRectangle() override = default;

            void setPosition(ecs::PositionComponent &pos) override;
            void setColor(std::tuple<int, int, int> &color) override;
            void setTexture(std::any texture) override;
            void setTextureRect(int offset_x, int offset_y, int length, int height, bool reversed) override;

            sf::RectangleShape shape;
    };
}
