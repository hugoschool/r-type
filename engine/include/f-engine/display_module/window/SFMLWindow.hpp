#pragma once

#include "f-engine/Exceptions.hpp"
#include "f-engine/display_module/shapes/IShape.hpp"
#include "f-engine/display_module/shapes/SFMLCircle.hpp"
#include "f-engine/display_module/shapes/SFMLRectangle.hpp"
#include "f-engine/display_module/window/IWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

#define WINDOW_X 1000
#define WINDOW_Y 800

namespace fengine::modules::display {
    class SFMLWindow : public IWindow {
        public:
            SFMLWindow() : _videoMode({WINDOW_X, WINDOW_Y}), _font() {
                try {
                    _window = sf::RenderWindow(_videoMode, "SFML window");
                    _window.setMaximumSize(_window.getSize());
                    _window.setMinimumSize(_window.getSize());
                } catch (std::exception &) { throw SfmlException("render window creation"); }
                _window.setFramerateLimit(60);
                // if (_font.openFromFile("textures/fonts/" FONT) == false) { // ajouter une font
                //     throw SfmlException("font creation");
                // }
            }

            ~SFMLWindow() override {
                _window.close();
            };

            void drawShape(std::shared_ptr<IShape> shape) override {
                if (auto rect = dynamic_cast<SFMLRectangle *>(shape.get())) {
                    _window.draw(rect->shape);
                }
                if (auto circle = dynamic_cast<SFMLCircle *>(shape.get())) {
                    _window.draw(circle->shape);
                }
            };

            void draw(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default) {
                _window.draw(drawable, states);
            };

            void display() {
                _window.display();
            };

            void clear() {
                _window.clear();
            };

            std::optional<sf::Event> pollEvent() {
                return _window.pollEvent();
            };

        private:
            sf::RenderWindow _window;
            sf::VideoMode _videoMode;
            sf::Font _font;
    };
}
