#include "f-engine/scene/GameScene.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"

fengine::scene::GameScene::GameScene(float cameraVelocity, std::optional<std::vector<Button>> buttons,
    std::string bgTexture, bool paralax)
    : _buttons(buttons), _background(), _cameraVelocity(cameraVelocity) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::GameScene::~GameScene() {
}

std::optional<std::vector<fengine::scene::Button>> &fengine::scene::GameScene::getButtons() {
    return _buttons;
}

fengine::scene::Background &fengine::scene::GameScene::getBackground() {
    return _background;
}

void fengine::scene::GameScene::update() {
    _background.offset += _cameraVelocity;
    if (_background.offset > WINDOW_X) {
        _background.offset = 0;
    }
}
