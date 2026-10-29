#include "f-engine/modules/display/shapes/SFMLCircle.hpp"

fengine::modules::display::SFMLCircle::SFMLCircle(float radius) {
    shape = sf::CircleShape();

    shape.setRadius(radius / 2);
};

void fengine::modules::display::SFMLCircle::SFMLCircle::setPosition(ecs::PositionComponent &pos) {
    shape.setPosition(sf::Vector2f(pos.pos_x, pos.pos_y));
};

void fengine::modules::display::SFMLCircle::SFMLCircle::setColor(std::tuple<int, int, int> &color) {
    int a = 255;
    int r = std::get<0>(color);
    int g = std::get<1>(color);
    int b = std::get<2>(color);
    sf::Color col = sf::Color(r, g, b, a);
    shape.setFillColor(col);
};

void fengine::modules::display::SFMLCircle::SFMLCircle::setTexture(std::any texture) {
    shape.setTexture(std::any_cast<sf::Texture *>(texture));
};

void fengine::modules::display::SFMLCircle::setTextureRect(
    int offset_x, int offset_y, int length, int height, bool reversed)
{
    sf::IntRect rect;
    rect.position = sf::Vector2i(offset_x, offset_y);
    rect.size = sf::Vector2i(length, height);

    if (!reversed) {
        rect.position.x += length;
        rect.size.x = -length;
    }

    shape.setTextureRect(rect);
}
