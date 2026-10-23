#include "f-engine/scene/AScene.hpp"
#include "f-engine/scene/SceneObjects.hpp"

fengine::scene::AScene::AScene(std::string bgTexture, bool paralax)
    : _eventQueue(), _buttons(), _background() {
    _background.offset = 0;
    _background.texture = bgTexture;
    _background.paralax = paralax;
}

fengine::scene::AScene::~AScene() {
}

void fengine::scene::AScene::updateButtons(std::pair<float, float> &coords) {
    if (_buttons.empty())
        return;

    for (auto &[type, button]: _buttons) {
        if (button.isInBounds(coords.first, coords.second)) {
            _eventQueue.push(type);
        }
    }
}

bool fengine::scene::AScene::empty() {
    return _eventQueue.empty();
}

int fengine::scene::AScene::pollEvents() {
    int type = _eventQueue.front();
    _eventQueue.pop();
    return type;
}

std::map<int, fengine::scene::Button> &fengine::scene::AScene::getButtons() {
    return _buttons;
}

fengine::scene::Background &fengine::scene::AScene::getBackground() {
    return _background;
}
