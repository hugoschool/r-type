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
#include "Exception.hpp"

rtype::client::Client::Client(): _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery() {
}

rtype::client::Client::~Client() {
}

void rtype::client::Client::run(char *av[]) {
    connect(av);
    loop();
}

    // TODO
void rtype::client::Client::connect(char *av[]) {
    // Connect to the server and send connection information to it.

    // the cient spams send the henshake to the server
    // when the client recieve the server response it stops
    // the server now sends all his ecs informations
    // the client now feed all these information into his ecs to match the server's one
}

void rtype::client::Client::loop() {
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
}
