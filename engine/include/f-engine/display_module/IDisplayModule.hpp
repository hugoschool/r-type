#pragma once

#include <memory>
#include <optional>
#include "events/IEvent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"

namespace fengine::modules::display {
    class IDisplayModule {
        public:
            virtual ~IDisplayModule() = default;

            virtual void clear() = 0;
            virtual void drawEntity(ecs::PositionComponent &, ecs::DrawableComponent &) = 0;

            virtual std::optional<std::unique_ptr<IEvent>> pollEvent() = 0;
    };
}
