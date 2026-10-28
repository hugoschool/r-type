#pragma once

#include "f-engine/config/IConfig.hpp"
#include "f-engine/modules/display/shapes/IShape.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>

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

            void changeConfig(std::shared_ptr<config::IConfig>) override;

            sf::Font font;

        private:
            sf::RenderTexture _scene;
            sf::Shader _colorblindShader;
            sf::RenderWindow _window;
            sf::VideoMode _videoMode;
            bool _colorblind;
    };
}
