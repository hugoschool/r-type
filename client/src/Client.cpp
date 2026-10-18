#include "Client.hpp"
#include "UDPProtocol.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/detail/error_code.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/AttackComponent.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/DefenseComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <thread>

rtype::client::Client::Client()
    : _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery(), _writeThread(),
      _readThread(), _exit(false), _inputs(1024), _protocol(std::make_unique<UDPProtocol>(_inputs)) {

    _registery.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::AnimationComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DrawableComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DeadComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::AttackComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DefenseComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
}

rtype::client::Client::~Client() {
    _writeThread.join();
    _readThread.join();
    _protocol->close();
}

void rtype::client::Client::run(char *av[]) {
    _protocol->connect(av);
    _writeThread = std::thread(&rtype::client::Client::sendLoop, this);
    _readThread = std::thread(&rtype::client::Client::readLoop, this);
    loop();
}

void rtype::client::Client::sendLoop() {
    while (!_exit) {
        _protocol->sendMsg();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "BYE BYE" << std::endl;
}

void rtype::client::Client::readLoop() {
    try {
        _protocol->setSocketNonBlocking();
        while (!_exit) {
            boost::system::error_code error = boost::asio::error::would_block;
            while (error == boost::asio::error::would_block && !_exit) {
                std::string msg = _protocol->readMsg(error);
                interpretMsg(msg);
            }
        }
    } catch (std::exception &) {}
}

void rtype::client::Client::interpretMsg(std::string msg) {
    packets::PacketHeader header;

    try {
        std::stringstream ss(msg);
        packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);
        packets::Serialization::deserialize(ar, header);
        switch (header.type) {
            // TODO: All packets
            case rtype::packets::PacketType::Ack:
                break;
            case rtype::packets::PacketType::InputPlayer:
                break;
            case rtype::packets::PacketType::EntityDied:
                removeEntity(ar);
                break;
            default:
                break;
        }
    } catch (std::exception &) {}
}

void rtype::client::Client::loop() {

    while (true) {
        _displayModule->clear();

        _registery.systemManager.update(_registery);

        std::optional<std::unique_ptr<fengine::modules::display::IEvent>> event = _displayModule->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == fengine::modules::display::EventType::Quit) {
                _exit = true;
                break;
            }
            if (event.value()->getType() == fengine::modules::display::EventType::KeyPressed) {
                _inputs.addElement(event.value()->getKey());
            }
        }
    }
}

void rtype::client::Client::removeEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    ar & id;
    try {
        _entities.erase(id.entityId);
    } catch (std::exception &) {
    }
}

void rtype::client::Client::addEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    ar & id;
    Entity newEntity = {id.entityId, {0, 0}, {0, 0}};
    _entities.insert({id.entityId, newEntity});
}
