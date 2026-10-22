#include "../../common/include/r-type/scene/GameScene.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/AScene.hpp"

fengine::scene::GameScene::GameScene(float cameraVelocity, std::optional<std::vector<Button>> buttons,
    std::string bgTexture, bool paralax) : fengine::scene::AScene(buttons, bgTexture, paralax), _cameraVelocity(cameraVelocity) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::GameScene::~GameScene() {
}

void fengine::scene::GameScene::update() {
    _background.offset += _cameraVelocity;
    if (_background.offset > WINDOW_X) {
        _background.offset = 0;
    }
}
