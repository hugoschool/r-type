#pragma once

#include "f-engine/modules/display/shapes/IShape.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

constexpr int WINDOW_X = 1000;
constexpr int WINDOW_Y = 800;

namespace fengine::modules::display {
    class SFMLWindow : public IWindow {
        public:
            SFMLWindow();
            ~SFMLWindow() override;

            void drawShape(std::shared_ptr<IShape> shape) override;
            void draw(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default);
            void display();

            void clear();
            std::optional<sf::Event> pollEvent();

            sf::Font font;

        private:
            sf::RenderWindow _window;
            sf::VideoMode _videoMode;
    };
}
