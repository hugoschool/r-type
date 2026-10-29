#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/ClickEvent.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "f-engine/modules/display/events/KeyPressedEvent.hpp"
#include "f-engine/modules/display/events/KeyReleasedEvent.hpp"
#include "f-engine/modules/display/events/QuitEvent.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/IScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <any>
#include <memory>
#include <optional>
#include <queue>

fengine::modules::display::SFMLDisplayModule::SFMLDisplayModule() : _window(), _textureMap(), _keyPressed() {
}

fengine::modules::display::SFMLDisplayModule::~SFMLDisplayModule() {
}

void fengine::modules::display::SFMLDisplayModule::clear() {
    _window.display();
    _window.clear();
}

std::optional<std::queue<std::unique_ptr<fengine::modules::display::IEvent>>>
fengine::modules::display::SFMLDisplayModule::pollEvent() {
    std::queue<std::unique_ptr<fengine::modules::display::IEvent>> eventQueue;
    while (std::optional event = _window.pollEvent()) {
        if (event.has_value() == false) {
            continue;
        }
        if (event->is<sf::Event::Closed>()) {
            eventQueue.push(std::make_unique<QuitEvent>());
        }
        if (const sf::Event::KeyPressed *key = event->getIf<sf::Event::KeyPressed>()) {
            if (_keyPressed.contains(key->code) == false) {
                _keyPressed.insert(key->code);
            }
        }
        if (const sf::Event::KeyReleased *key = event->getIf<sf::Event::KeyReleased>()) {
            if (_keyPressed.contains(key->code) == true) {
                _keyPressed.erase(key->code);
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
                    break;
            }
            eventQueue.push(std::make_unique<ClickEvent>(mouseButton, mouse->position.x, mouse->position.y));
        }
    }
    for (auto keyCode : _keyPressed) {
        if (keyCode >= sf::Keyboard::Key::A && keyCode <= sf::Keyboard::Key::Z) {
            eventQueue.push(std::make_unique<KeyPressedEvent>(static_cast<EventKey>(static_cast<int>(keyCode) + 1)));
        } else {
            eventQueue.push(std::make_unique<KeyPressedEvent>(interpretKeyCode(keyCode)));
        }
    }
    if (eventQueue.empty()) {
        return std::nullopt;
    } else {
        return eventQueue;
    }
}

fengine::modules::display::EventKey fengine::modules::display::SFMLDisplayModule::interpretKeyCode(
    sf::Keyboard::Key code) const {
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

void fengine::modules::display::SFMLDisplayModule::drawScene(std::shared_ptr<fengine::scene::IScene> scene) {
    std::map<int, scene::Button> &buttons = scene->getButtons();
    scene::Background &background =
        scene
            ->getBackground();// a voir comment definir cela, pour l'instant surement une shape rectangle de la taille de la window + un offset handle dans la scene elle meme

    // background (a changer aussi)
    sf::RectangleShape bg1 = sf::RectangleShape(sf::Vector2f(WINDOW_X, WINDOW_Y));
    if (_textureMap.contains(background.texture) == false) {
        sf::Texture textureBg;
        if (textureBg.loadFromFile(background.texture))
            _textureMap.insert({background.texture, std::move(textureBg)});
    }
    auto &t = _textureMap.at(background.texture).value();
    bg1.setPosition(sf::Vector2f(-background.offset, 0));
    bg1.setTexture(&t);
    _window.draw(bg1);
    if (background.paralax) {
        sf::RectangleShape bg2 = sf::RectangleShape(sf::Vector2f(WINDOW_X, WINDOW_Y));
        bg2.setPosition(sf::Vector2f(-background.offset + WINDOW_X, 0));
        bg2.setTexture(&t);
        _window.draw(bg2);
    }

    // boutons (a changer)
    for (auto &[type, button] : buttons) {
        sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(button.length, button.height));
        rect.setPosition(sf::Vector2f(button.pos_x, button.pos_y));

        sf::Texture texture;

        if (_textureMap.contains(button.content) == false) {
            if (texture.loadFromFile(button.content)) {
                _textureMap.insert({button.content, std::move(texture)});
            } else {
                _textureMap.insert({button.content, std::nullopt});
            }
        }

        auto &t = _textureMap.at(button.content);
        if (t.has_value()) {
            rect.setTexture(&t.value());
            _window.draw(rect);
        } else {
            sf::Text text(_window.font);
            text.setString(button.content);
            text.setPosition(sf::Vector2f(button.pos_x + 10, button.pos_y + 10));
            text.setFillColor(sf::Color::Black);
            text.setCharacterSize(20);
            _window.draw(rect);
            _window.draw(text);
        }
    }
}

void fengine::modules::display::SFMLDisplayModule::drawEntity(fengine::ecs::PositionComponent pos,
    fengine::ecs::DrawableComponent &drawable) {
    if (!drawable.shape)
        return;

    std::optional<sf::Color> color = std::nullopt;

    if (drawable.color.has_value()) {
        int a = 255;
        int r = std::get<0>(drawable.color.value());
        int g = std::get<1>(drawable.color.value());
        int b = std::get<2>(drawable.color.value());
        color = sf::Color(r, g, b, a);
    }

    if (drawable.texture_filepath.has_value()) {
        if (_textureMap.contains(drawable.texture_filepath.value()) == false) {
            sf::Texture texture;

            if (texture.loadFromFile(drawable.texture_filepath.value())) {
                _textureMap.insert({drawable.texture_filepath.value(), std::move(texture)});
            } else {
                _textureMap.insert({drawable.texture_filepath.value(), std::nullopt});
            }
        }
        drawable.shape->setTexture(&_textureMap.at(drawable.texture_filepath.value()).value());
    }
    if (drawable.color.has_value()) {
        drawable.shape->setColor(drawable.color.value());
    }
    drawable.shape->setPosition(pos);
    _window.drawShape(drawable.shape);
}

void fengine::modules::display::SFMLDisplayModule::animateEntity(ecs::AnimationComponent &anim,
    ecs::DrawableComponent &drawable) {
    drawable.shape->setTextureRect(anim.offset_x, anim.offset_y, anim.length, anim.height);
}

void fengine::modules::display::SFMLDisplayModule::verifyConfig() {
    if (_config->hasChanged()) {
        _window.changeConfig(_config);
        _config->resetChange();
    }
}
