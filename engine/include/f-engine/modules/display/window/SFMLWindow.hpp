#pragma once

#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include "f-engine/config/IConfig.hpp"
#include "f-engine/modules/display/shapes/IShape.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <memory>

constexpr std::string_view FONT = "./assets/fonts/PressStart2P.ttf";
constexpr std::string_view CSHADER = "./assets/shaders/colorblind.glsl";

namespace fengine::modules::display {
    class SFMLWindow : public IWindow {
        public:
            SFMLWindow(std::shared_ptr<asset_manager::SFMLAssetManager> assetManager);
            ~SFMLWindow() override;

            void drawShape(std::shared_ptr<IShape> shape) override;
            void draw(const sf::Drawable &drawable, const sf::RenderStates &states = sf::RenderStates::Default);
            void display();

            void clear();
            std::optional<sf::Event> pollEvent();

            void changeConfig(std::shared_ptr<config::IConfig>) override;
        private:
            sf::RenderTexture _scene;
            sf::RenderWindow _window;
            sf::VideoMode _videoMode;
            bool _colorblind;
            std::shared_ptr<asset_manager::SFMLAssetManager> _assetManager;
    };
}
