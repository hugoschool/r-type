#include "r-type/scene/MenuScene.hpp"
#include "f-engine/scene/AScene.hpp"

fengine::scene::MenuScene::MenuScene(std::optional<std::vector<Button>> buttons,
    std::string bgTexture, bool paralax) : fengine::scene::AScene(buttons, bgTexture, paralax) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::MenuScene::~MenuScene() {
}

void fengine::scene::MenuScene::update() {
    return;
}
