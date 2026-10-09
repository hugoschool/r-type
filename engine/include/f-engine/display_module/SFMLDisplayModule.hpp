#pragma once

#include "f-engine/display_module/IDisplayModule.hpp"
#include "f-engine/display_module/window/SFMLWindow.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <map>

namespace fengine::modules::display {
    class SFMLDisplayModule : public IDisplayModule {
        public:
            SFMLDisplayModule();
            ~SFMLDisplayModule();

            void clear() override;
            void drawEntity(ecs::PositionComponent &, ecs::DrawableComponent &) override;

            std::optional<std::unique_ptr<IEvent>> pollEvent() override;

        private:
            EventKey interpretKeyCode(sf::Keyboard::Key code) const;

            SFMLWindow _window;

            std::map<std::string, std::optional<sf::Texture>> _textureMap;
    };
}
