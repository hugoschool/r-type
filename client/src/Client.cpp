#include "Client.hpp"
#include "f-engine/display_module/IDisplayModule.hpp"
#include "f-engine/display_module/SFMLDisplayModule.hpp"
#include <memory>
#include "f-engine/display_module/SFMLDisplayModule.hpp"
#include "f-engine/display_module/events/EventKey.hpp"
#include "f-engine/display_module/events/EventType.hpp"
#include "f-engine/display_module/events/IEvent.hpp"
#include "f-engine/display_module/shapes/SFMLRectangle.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/systems/BoundarySystem.hpp"
#include "f-engine/ecs/systems/ControlSystem.hpp"
#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/ecs/systems/MoveSystem.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <tuple>


rtype::client::Client::Client(): _displayModule(new fengine::modules::display::SFMLDisplayModule), _registery() {
}

rtype::client::Client::~Client() {
    // maybe delete what was created
}

int rtype::client::Client::run(char *av[]) {
    if (this->connect(av) == 84) {
        return 84;
    }

    this->loop();
    return 0;
}

int rtype::client::Client::connect(char *av[]) {
    // TODO
    return 0;
}

int rtype::client::Client::loop() {
    _registery.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DrawableComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::ControllableComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();

    float posx = 0;
    float posy = 0;
    float vel_x = 8;
    float vel_y = 7;
    for (int i = 0; i < 1; i++) {
        fengine::ecs::Entity entity = _registery.entityManager.spawnEntity();
        fengine::ecs::PositionComponent pos = {posx, posy};
        fengine::ecs::VelocityComponent vel = {vel_x, vel_y};
        fengine::ecs::BoundaryComponent bound = {40, 40, false, false};
        fengine::ecs::DrawableComponent draw = {std::nullopt, std::make_shared<fengine::modules::display::SFMLCircle>(bound.size_x), std::tuple<int, int, int>(rand()%255, rand()%255, rand()%255)};

        _registery.componentManager.addComponent(entity, std::move(pos));
        _registery.componentManager.addComponent(entity, std::move(vel));
        _registery.componentManager.addComponent(entity, std::move(draw));
        _registery.componentManager.addComponent(entity, std::move(bound));
        posx += 0.01;
        posy += 0.01;
        if (vel_x > 0)
            vel_x += 0.01;
        vel_x *= -1;
        if (vel_y > 0)
            vel_y += 0.01;
        vel_y *= -1;
    }

    _registery.systemManager.addSystem<fengine::ecs::MovementSystem>();
    _registery.systemManager.addSystem<fengine::ecs::BoundarySystem>();
    _registery.systemManager.addSystem<fengine::ecs::DisplaySystem>(*_displayModule);

    while (true) {
        // get player input
        // send it to the server
        // create prediction based on the input
        // get server response compare server response with the predition
        _displayModule->clear();

        _registery.systemManager.update(_registery);

        std::optional<std::unique_ptr<fengine::modules::display::IEvent>> event = _displayModule->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == fengine::modules::display::EventType::Quit) {
                break;
            }
        }
    }
    return 0;
}
