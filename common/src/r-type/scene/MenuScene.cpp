#include "r-type/scene/MenuScene.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/AScene.hpp"

fengine::scene::MenuScene::MenuScene(std::string bgTexture, bool paralax) : fengine::scene::AScene(bgTexture, paralax) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;

    _buttons.insert(
        {static_cast<int>(MenuSceneButtons::PLAY), Button(200, 150, WINDOW_X / 2 - 100, WINDOW_Y / 2, "PLAY")});
    _buttons.insert({static_cast<int>(MenuSceneButtons::QUIT), Button(200, 150, 100, WINDOW_Y - 200, "QUIT")});
    _buttons.insert({static_cast<int>(MenuSceneButtons::PARAMS), Button(200, 150, WINDOW_X - 250, 100, "PARAMS")});
}

fengine::scene::MenuScene::~MenuScene() {
}

void fengine::scene::MenuScene::update() {
    return;
}
