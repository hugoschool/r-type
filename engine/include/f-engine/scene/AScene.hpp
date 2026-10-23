#pragma once

#include "f-engine/scene/IScene.hpp"
#include <queue>
namespace fengine::scene {
    class AScene : public IScene {
        public:
            AScene(std::string bgTexture, bool paralax);
            ~AScene() override;

            std::map<int, Button> &getButtons() override;

            Background &getBackground() override;

            void updateButtons(std::pair<float, float> &) override;

            bool empty() override;
            int pollEvents() override;

        protected:
            std::queue<int> _eventQueue;
            std::map<int, Button> _buttons;
            Background _background;
    };
}
