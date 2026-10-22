#include "r-type/scene/ParamScene.hpp"
#include "f-engine/scene/AScene.hpp"

fengine::scene::ParamScene::ParamScene(std::optional<std::vector<Button>> buttons, std::string bgTexture, bool paralax)
    : fengine::scene::AScene(buttons, bgTexture, paralax) {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::ParamScene::~ParamScene() {
}

void fengine::scene::ParamScene::update() {
    return;
}
