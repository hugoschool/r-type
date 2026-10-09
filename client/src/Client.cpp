#include "Client.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <iostream>
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

rtype::client::Client::Client() : _io_context(), _resolver(_io_context), _endpoint(), _socket(_io_context), _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery() {
    const std::string host = "127.0.0.1";
    const std::string port = "25000";
    _endpoint = *_resolver.resolve(udp::v4(), host, port).begin();

    _socket.open(udp::v4());
    connect();
}

rtype::client::Client::~Client() {
}

int rtype::client::Client::run(char *av[]) {
    if (this->connect(av) == 84) {
        return 84;
    }

    this->loop();
    return 0;
}

int rtype::client::Client::connect(char *av[]) {
    std::stringstream ss;

    packets::PacketHeader header = {
        .type = packets::PacketType::Ack,
        .size = 0,
        // TODO: this is a simple test UUID for the server <-> client communication
        // To be removed later
        .playerUuid = "f52ea348-6c57-4acf-9834-059bc9a9c81c",
    };

    packets::Serialization::serialize(ss, header, packets::Empty{});

    std::string send = ss.str();
    _socket.send_to(boost::asio::buffer(send), _endpoint);

    while (true) {
        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = _socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }
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
