#pragma once

#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/modules/display/ADisplayModule.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/IScene.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <map>

namespace fengine::modules::display {
    class SFMLDisplayModule : public ADisplayModule {
        public:
            SFMLDisplayModule();
            ~SFMLDisplayModule() override;

            void clear() override;
            void drawEntity(ecs::PositionComponent, ecs::DrawableComponent &) override;

            void animateEntity(ecs::AnimationComponent &, ecs::DrawableComponent &) override;

            void drawScene(std::shared_ptr<scene::IScene>) override;

            std::optional<std::unique_ptr<IEvent>> pollEvent() override;

            void verifyConfig() override;

        private:
            EventKey interpretKeyCode(sf::Keyboard::Key code) const;

            SFMLWindow _window;

            std::map<std::string, std::optional<sf::Texture>> _textureMap;
    };
}
