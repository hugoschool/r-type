#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/Exceptions.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"

fengine::modules::display::SFMLWindow::SFMLWindow() : font(), _videoMode({WINDOW_X, WINDOW_Y}) {
    try {
        _window = sf::RenderWindow(_videoMode, "R-Type");
        _window.setMaximumSize(_window.getSize());
        _window.setMinimumSize(_window.getSize());
    } catch (std::exception &) { throw SfmlException("render window creation"); }
    _window.setFramerateLimit(60);
    // if (font.openFromFile("textures/fonts/" FONT) == false) { // ajouter une font
    //     throw SfmlException("font creation");
    // }
}

fengine::modules::display::SFMLWindow::~SFMLWindow() {
    _window.close();
};

void fengine::modules::display::SFMLWindow::drawShape(std::shared_ptr<IShape> shape) {
    if (auto rect = dynamic_cast<SFMLRectangle *>(shape.get())) {
        _window.draw(rect->shape);
    }
    if (auto circle = dynamic_cast<SFMLCircle *>(shape.get())) {
        _window.draw(circle->shape);
    }
};

void fengine::modules::display::SFMLWindow::draw(const sf::Drawable &drawable, const sf::RenderStates &states) {
    _window.draw(drawable, states);
};

void fengine::modules::display::SFMLWindow::display() {
    _window.display();
};

void fengine::modules::display::SFMLWindow::clear() {
    _window.clear();
};

std::optional<sf::Event> fengine::modules::display::SFMLWindow::pollEvent() {
    return _window.pollEvent();
};
