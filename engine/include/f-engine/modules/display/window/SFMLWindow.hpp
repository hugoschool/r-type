#pragma once

#include "f-engine/Exceptions.hpp"
#include "f-engine/modules/display/shapes/IShape.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

constexpr int WINDOW_X = 1000;
constexpr int WINDOW_Y = 800;

namespace fengine::modules::display {
    class SFMLWindow : public IWindow {
        public:
            SFMLWindow() : font(), _videoMode({WINDOW_X, WINDOW_Y}) {
                try {
                    _window = sf::RenderWindow(_videoMode, "R-Type");
                    _window.setMaximumSize(_window.getSize());
                    _window.setMinimumSize(_window.getSize());
                } catch (std::exception &) { throw SfmlException("render window creation"); }
                if (!sf::Shader::isAvailable()) {
                    throw SfmlException("shaders are unavailable");
                }
                if (!_colorblindShader.loadFromFile("./assets/shaders/colorblind.glsl", sf::Shader::Type::Fragment)) {
                    throw SfmlException("failed to load colorblind shader");
                }
                // change that in the future to have a client configuration object
                _colorblind = false;
                _colorblindShader.setUniform("mode", 3);
                _window.setFramerateLimit(60);
                _scene = sf::RenderTexture(sf::Vector2u(WINDOW_X, WINDOW_Y));
                if (!font.openFromFile("./assets/fonts/PressStart2P.ttf")) {
                    throw SfmlException("failed to load font");
                }
            }

            ~SFMLWindow() override {
                _window.close();
            };

            void drawShape(std::shared_ptr<IShape> shape) override {
                if (auto rect = dynamic_cast<SFMLRectangle *>(shape.get())) {
                    _scene.draw(rect->shape);
                }
                if (auto circle = dynamic_cast<SFMLCircle *>(shape.get())) {
                    _scene.draw(circle->shape);
                }
            };

            void draw(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default) {
                _scene.draw(drawable, states);
            };

            void display() {
                _scene.display();
                sf::Sprite sceneSprite(_scene.getTexture());
                if (_colorblind) {
                    _window.draw(sceneSprite, &_colorblindShader);
                } else {
                    _window.draw(sceneSprite);
                }
                _window.display();
            };

            void clear() {
                _scene.clear(sf::Color::Black);
                _window.clear();
            };

            std::optional<sf::Event> pollEvent() {
                return _window.pollEvent();
            };

            sf::Font font;

        private:
            sf::RenderTexture _scene;
            sf::Shader _colorblindShader;
            sf::RenderWindow _window;
            sf::VideoMode _videoMode;
            bool _colorblind;
    };
}
