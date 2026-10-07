#include "f-engine/display_module/SFMLDisplayModule.hpp"
#include "f-engine/display_module/events/IEvent.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <optional>
#include "f-engine/display_module/events/KeyReleasedEvent.hpp"
#include "f-engine/display_module/events/QuitEvent.hpp"
#include "f-engine/display_module/events/ClickEvent.hpp"
#include "f-engine/display_module/events/KeyPressedEvent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"

fengine::modules::display::SFMLDisplayModule::SFMLDisplayModule(): _window(), _textureMap(), _spriteMap()
{}

fengine::modules::display::SFMLDisplayModule::~SFMLDisplayModule()
{}

void fengine::modules::display::SFMLDisplayModule::clear()
{
    _window.display();
    _window.clear();
}

std::optional<std::unique_ptr<fengine::modules::display::IEvent>> fengine::modules::display::SFMLDisplayModule::pollEvent()
{
    while (std::optional event = _window.pollEvent()) {
        if (event.has_value() == false) {
            continue;
        }
        if (event->is<sf::Event::Closed>()) {
            return std::make_unique<QuitEvent>();
        }
        if (const sf::Event::KeyPressed *key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code >= sf::Keyboard::Key::A && key->code <= sf::Keyboard::Key::Z) {
                return std::make_unique<KeyPressedEvent>(static_cast<EventKey>(static_cast<int>(key->code) + 1));
            } else {
                return std::make_unique<KeyPressedEvent>(interpretKeyCode(key->code));
            }
        }
        if (const sf::Event::KeyReleased *key = event->getIf<sf::Event::KeyReleased>()) {
            if (key->code >= sf::Keyboard::Key::A && key->code <= sf::Keyboard::Key::Z) {
                return std::make_unique<KeyReleasedEvent>(static_cast<EventKey>(static_cast<int>(key->code) + 1));
            } else {
                return std::make_unique<KeyReleasedEvent>(interpretKeyCode(key->code));
            }
        }
        if (const sf::Event::MouseButtonPressed *mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            EventMouseButton mouseButton;
            switch (mouse->button) {
                case sf::Mouse::Button::Left:
                    mouseButton = EventMouseButton::Left;
                    break;
                case sf::Mouse::Button::Right:
                    mouseButton = EventMouseButton::Right;
                    break;
                default:
                    return std::nullopt;
            }
            return std::make_unique<ClickEvent>(mouseButton);
        }
    }
    return std::nullopt;
}

fengine::modules::display::EventKey fengine::modules::display::SFMLDisplayModule::interpretKeyCode(sf::Keyboard::Key code) const
{
    switch (code) {
        case sf::Keyboard::Key::Num0:
            return EventKey::_0;
        case sf::Keyboard::Key::Num1:
            return EventKey::_1;
        case sf::Keyboard::Key::Num2:
            return EventKey::_2;
        case sf::Keyboard::Key::Num3:
            return EventKey::_3;
        case sf::Keyboard::Key::Num4:
            return EventKey::_4;
        case sf::Keyboard::Key::Num5:
            return EventKey::_5;
        case sf::Keyboard::Key::Num6:
            return EventKey::_6;
        case sf::Keyboard::Key::Num7:
            return EventKey::_7;
        case sf::Keyboard::Key::Num8:
            return EventKey::_8;
        case sf::Keyboard::Key::Num9:
            return EventKey::_9;
        case sf::Keyboard::Key::Enter:
            return EventKey::Enter;
        case sf::Keyboard::Key::Space:
            return EventKey::Space;
        case sf::Keyboard::Key::Backspace:
            return EventKey::Backspace;
        case sf::Keyboard::Key::Left:
            return EventKey::Left;
        case sf::Keyboard::Key::Right:
            return EventKey::Right;
        case sf::Keyboard::Key::Up:
            return EventKey::Up;
        case sf::Keyboard::Key::Down:
            return EventKey::Down;
        default:
            return EventKey::None;
    }
}

void fengine::modules::display::SFMLDisplayModule::drawEntity(fengine::ecs::PositionComponent &pos, fengine::ecs::DrawableComponent &drawable)
{
    std::optional<sf::Color> color = std::nullopt;

    if (drawable.color.has_value()) {
        int a = 255;
        int r = std::get<0>(drawable.color.value());
        int g = std::get<1>(drawable.color.value());
        int b = std::get<2>(drawable.color.value());
        color = sf::Color(r, g, b, a);
    }

    if (drawable.texture_filepath.has_value()) {
        if (_textureMap.contains(drawable.texture_filepath.value()) == false && _spriteMap.contains(drawable.texture_filepath.value()) == false) {
            sf::Texture texture;

            if (texture.loadFromFile(drawable.texture_filepath.value())) {
                _textureMap.insert({drawable.texture_filepath.value(), std::make_unique<sf::Texture>(texture)});
                sf::Sprite sprite(texture);
                if (color.has_value())
                    sprite.setColor(color.value());
                sprite.setPosition(sf::Vector2f({pos.pos_x, pos.pos_y}));
                _spriteMap.insert({drawable.texture_filepath.value(), std::make_unique<sf::Sprite>(sprite)});
                _window.draw(sprite);
            } else {
                _textureMap.insert({drawable.texture_filepath.value(), std::nullopt});
                _spriteMap.insert({drawable.texture_filepath.value(), std::nullopt});
            }
        } else {
            std::optional<std::unique_ptr<sf::Sprite>> sprite = std::move(_spriteMap.at(drawable.texture_filepath.value()));
            if (sprite.has_value())
                _window.draw(*sprite.value().get());
        }
        return;
    }

    if (drawable.shape.has_value()) {
        if (drawable.color.has_value()) {
            drawable.shape.value()->setColor(drawable.color.value());
        }
        drawable.shape.value()->setPosition(pos);
        _window.drawShape(drawable.shape.value());
    }
}
