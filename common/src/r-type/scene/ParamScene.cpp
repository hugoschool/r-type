#include "r-type/scene/ParamScene.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "f-engine/scene/AScene.hpp"

fengine::scene::ParamScene::ParamScene(std::string bgTexture, bool paralax)
    : fengine::scene::AScene(bgTexture, paralax) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;

    _buttons.insert({static_cast<int>(ParamSceneButtons::MENU), Button(200, 150, WINDOW_X - 250, 100, "MENU")});
    _buttons.insert({static_cast<int>(ParamSceneButtons::COLORBLIND_1), Button(200, 150, 100, 100, "COLORBLIND 1")});
    _buttons.insert({static_cast<int>(ParamSceneButtons::COLORBLIND_2), Button(200, 150, 100, 300, "COLORBLIND 2")});
    _buttons.insert({static_cast<int>(ParamSceneButtons::COLORBLIND_3), Button(200, 150, 100, 500, "COLORBLIND 3")});
}

fengine::scene::ParamScene::~ParamScene() {
}

void fengine::scene::ParamScene::update() {
    return;
}
