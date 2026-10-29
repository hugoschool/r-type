#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"

fengine::modules::display::SFMLRectangle::SFMLRectangle(float width, float height) {
    shape = sf::RectangleShape();

    shape.setSize(sf::Vector2f(width, height));
};

void fengine::modules::display::SFMLRectangle::setPosition(ecs::PositionComponent &pos) {
    shape.setPosition(sf::Vector2f(pos.pos_x, pos.pos_y));
};

void fengine::modules::display::SFMLRectangle::setColor(std::tuple<int, int, int> &color) {
    int a = 255;
    int r = std::get<0>(color);
    int g = std::get<1>(color);
    int b = std::get<2>(color);
    sf::Color col = sf::Color(r, g, b, a);
    shape.setFillColor(col);
};

void fengine::modules::display::SFMLRectangle::setTexture(std::any texture) {
    shape.setTexture(std::any_cast<sf::Texture *>(texture));
};

void fengine::modules::display::SFMLRectangle::setTextureRect(int offset_x, int offset_y, int length, int height,
    bool reversed) {
    sf::IntRect rect;
    rect.position = sf::Vector2i(offset_x, offset_y);
    rect.size = sf::Vector2i(length, height);

    if (!reversed) {
        rect.position.x += length;
        rect.size.x = -length;
    }

    shape.setTextureRect(rect);
}
