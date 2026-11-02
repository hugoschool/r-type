#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/Exceptions.hpp"
#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include "f-engine/config/IConfig.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include <SFML/Graphics/Sprite.hpp>

fengine::modules::display::SFMLWindow::SFMLWindow(
    std::shared_ptr<fengine::asset_manager::SFMLAssetManager> assetManager)
    : _videoMode({WINDOW_X, WINDOW_Y}), _colorblind(false), _assetManager(assetManager) {
    try {
        _window = sf::RenderWindow(_videoMode, "R-Type");
        _window.setMaximumSize(_window.getSize());
        _window.setMinimumSize(_window.getSize());
        _window.setKeyRepeatEnabled(false);
    } catch (std::exception &) { throw SfmlException("render window creation"); }
    _window.setFramerateLimit(60);
    _assetManager->loadFont(std::string(FONT));
    _assetManager->loadShader(std::string(CSHADER));
    _scene = sf::RenderTexture(sf::Vector2u(WINDOW_X, WINDOW_Y));
}

fengine::modules::display::SFMLWindow::~SFMLWindow() {
    _window.close();
};

void fengine::modules::display::SFMLWindow::drawShape(std::shared_ptr<IShape> shape) {
    if (auto rect = dynamic_cast<SFMLRectangle *>(shape.get())) {
        _scene.draw(rect->shape);
    }
    if (auto circle = dynamic_cast<SFMLCircle *>(shape.get())) {
        _scene.draw(circle->shape);
    }
};

void fengine::modules::display::SFMLWindow::draw(const sf::Drawable &drawable, const sf::RenderStates &states) {
    _scene.draw(drawable, states);
};

void fengine::modules::display::SFMLWindow::display() {
    _scene.display();
    sf::Sprite sceneSprite(_scene.getTexture());
    if (_colorblind) {
        _window.draw(sceneSprite, &_assetManager->getShader(std::string(CSHADER)).value());
    } else {
        _window.draw(sceneSprite);
    }
    _window.display();
};

void fengine::modules::display::SFMLWindow::clear() {
    _scene.clear(sf::Color::Black);
    _window.clear();
};

std::optional<sf::Event> fengine::modules::display::SFMLWindow::pollEvent() {
    return _window.pollEvent();
};

void fengine::modules::display::SFMLWindow::changeConfig(std::shared_ptr<config::IConfig> config) {
    auto colorblindConfig = config->get("colorblind");
    if (colorblindConfig->has_value()) {
        auto value = std::any_cast<std::pair<bool, int>>(colorblindConfig.value());
        // probably change this because right now its not great to do this if
        if (_colorblind)
            _colorblind = false;
        else {
            _colorblind = value.first;
            _assetManager->getShader(std::string(CSHADER)).value().setUniform("mode", value.second);
        }
    }
}
