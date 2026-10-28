#pragma once

#include "events/IEvent.hpp"
#include "f-engine/config/IConfig.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/scene/IScene.hpp"
#include <memory>
#include <optional>
#include <queue>

namespace fengine::modules::display {
    class IDisplayModule {
        public:
            virtual ~IDisplayModule() = default;

            virtual void clear() = 0;
            virtual void drawEntity(ecs::PositionComponent, ecs::DrawableComponent &) = 0;

            virtual void animateEntity(ecs::AnimationComponent &, ecs::DrawableComponent &) = 0;

            virtual void drawScene(std::shared_ptr<scene::IScene>) = 0;

            virtual std::optional<std::queue<std::unique_ptr<IEvent>>> pollEvent() = 0;

            virtual void setConfig(std::shared_ptr<config::IConfig>) = 0;

            virtual void verifyConfig() = 0;
    };
}
