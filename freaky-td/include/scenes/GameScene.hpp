#pragma once

#include "f-engine/scene/AScene.hpp"
#include <string>

namespace freakytd {
    class GameScene : public fengine::scene::AScene {
        public:
            GameScene(std::string, bool, int &, int &);
            ~GameScene();

            void update() override;
        private:
            int &_money;
            int &_lives;
    };
}
