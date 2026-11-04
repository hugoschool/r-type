#include "r-type/scene/GameScene.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <cstddef>
#include <string>

fengine::scene::GameScene::GameScene(float cameraVelocity, std::string bgTexture, bool paralax, std::size_t &level)
    : fengine::scene::AScene(bgTexture, paralax), _cameraVelocity(cameraVelocity), _level(level) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;

    scene::Text text = {
        .content = std::string("Level: ") + std::to_string(_level),
        .pos_x = 20,
        .pos_y = 20,
    };

    _texts.push_back(text);
}

fengine::scene::GameScene::~GameScene() {
}

void fengine::scene::GameScene::update() {
    _background.offset += _cameraVelocity;
    if (_background.offset > WINDOW_X) {
        _background.offset = 0;
    }

    _texts.at(0).content = std::string("Level: ") + std::to_string(_level);
}
