#include "scenes/GameScene.hpp"

freakytd::GameScene::GameScene(std::string bgTexture, bool paralax, int &money, int &lives): fengine::scene::AScene(bgTexture, paralax), _money(money), _lives(lives) {
    _texts.push_back({
        .content = std::string("Lives: ") + std::to_string(_lives),
        .pos_x = 20,
        .pos_y = 20,
    });
    _texts.push_back({
        .content = std::string("Money: ") + std::to_string(_money),
        .pos_x = 250,
        .pos_y = 20,
    });

}

freakytd::GameScene::~GameScene() {}

void freakytd::GameScene::update() {
    _texts.at(0).content = std::string("Lives: ") + std::to_string(_lives);
    _texts.at(1).content = std::string("Money: ") + std::to_string(_money);
}
