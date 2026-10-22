#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"
#include <vector>

fengine::scene::AScene::AScene(std::optional<std::vector<Button>> buttons, std::string bgTexture, bool paralax)
    : _buttons(buttons), _background() {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::AScene::~AScene() {
}

std::optional<std::vector<fengine::scene::Button>> &fengine::scene::AScene::getButtons() {
    return _buttons;
}

fengine::scene::Background &fengine::scene::AScene::getBackground() {
    return _background;
}
