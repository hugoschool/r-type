#include "Client.hpp"
#include "UDPProtocol.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/detail/error_code.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/AttackComponent.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/DefenseComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/systems/BoundarySystem.hpp"
#include "f-engine/ecs/systems/ControlSystem.hpp"
#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/ecs/systems/MoveSystem.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include "f-engine/scene/GameScene.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

rtype::client::Client::Client()
    : _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery(),
      _camera(0.0, 100000), _scene(3.0, std::nullopt, "./assets/test.png", true),
      _entity(_registery.entityManager.spawnEntity()), _writeThread(), _readThread(), _exit(false), _inputs(1024),
      _protocol(std::make_unique<UDPProtocol>(_inputs)) {

    _registery.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::AnimationComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DrawableComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DeadComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::AttackComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::DefenseComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::ControllableComponent>();

    fengine::ecs::BoundaryComponent bound = {100, 50, true, false, std::nullopt};
    fengine::ecs::PositionComponent pos = {0, 0, false};
    fengine::ecs::VelocityComponent vel = {0, 0};
    fengine::ecs::DrawableComponent draw = {std::nullopt,
        std::make_shared<fengine::modules::display::SFMLCircle>(bound.size_x), std::nullopt};
    fengine::ecs::ControllableComponent control = {fengine::modules::display::EventKey::Z,
        fengine::modules::display::EventKey::Q, fengine::modules::display::EventKey::S,
        fengine::modules::display::EventKey::D};
    fengine::ecs::AttackComponent att = {{.team = fengine::ecs::team::ALLIES}, 10};
    fengine::ecs::DefenseComponent def = {100, 0};

    _registery.componentManager.addComponent(_entity, std::move(pos));
    _registery.componentManager.addComponent(_entity, std::move(vel));
    _registery.componentManager.addComponent(_entity, std::move(control));
    _registery.componentManager.addComponent(_entity, std::move(att));
    _registery.componentManager.addComponent(_entity, std::move(def));
    _registery.componentManager.addComponent(_entity, std::move(draw));
    _registery.componentManager.addComponent(_entity, std::move(bound));

    _pressqueue = std::queue<fengine::modules::display::EventKey>();
    _releasequeue = std::queue<fengine::modules::display::EventKey>();

    _registery.systemManager.addSystem<fengine::ecs::MovementSystem>(_camera);
    _registery.systemManager.addSystem<fengine::ecs::BoundarySystem>();
    _registery.systemManager.addSystem<fengine::ecs::ControlSystem>(_pressqueue, _releasequeue);
    _registery.systemManager.addSystem<fengine::ecs::DisplaySystem>(*_displayModule, _camera);
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
            case rtype::packets::PacketType::EntitySpawn:
                addEntity(ar);
                break;
            case rtype::packets::PacketType::EntityMove:
                moveEntity(ar);
                break;
            case rtype::packets::PacketType::EntityVelocityChange:
                modifyVelocity(ar);
                break;
            default:
                break;
        }
    } catch (std::exception &) {}
}

void rtype::client::Client::loop() {

    while (true) {
        _displayModule->clear();

        _displayModule->drawScene(_scene);
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
        _scene.update();
    }
}

void rtype::client::Client::removeEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    ar & id;
    try {
        auto &entity = _registery.entityManager.getEntities().at(id.entityId);
        _registery.entityManager.killEntity(entity.value());
    } catch (std::exception &) {}
}

void rtype::client::Client::addEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    ar & id;

    try {
        auto &_ = _registery.entityManager.getEntities().at(id.entityId);
    } catch (std::exception &) {
        fengine::ecs::Entity entity = _registery.entityManager.createEntity(id.entityId);
        fengine::ecs::PositionComponent pos = {200, 200, false};
        fengine::ecs::VelocityComponent vel = {0, 0};
        fengine::ecs::BoundaryComponent bound = {40, 40, true, true, std::nullopt};
        fengine::ecs::DrawableComponent draw = {std::nullopt,
            std::make_shared<fengine::modules::display::SFMLRectangle>(bound.size_x, bound.size_y), std::nullopt};
        fengine::ecs::AttackComponent att = {{.team = fengine::ecs::team::MONSTERS}, 10};
        fengine::ecs::DefenseComponent def = {100, 0};

        _registery.componentManager.addComponent(entity, std::move(pos));
        _registery.componentManager.addComponent(entity, std::move(vel));
        _registery.componentManager.addComponent(entity, std::move(draw));
        _registery.componentManager.addComponent(entity, std::move(bound));
        _registery.componentManager.addComponent(entity, std::move(att));
        _registery.componentManager.addComponent(entity, std::move(def));
    }
}

void rtype::client::Client::moveEntity(rtype::packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    packets::Vector2D pos;
    ar & id;
    ar & pos;
    try {
        auto &sparseArray = _registery.componentManager.getComponents<fengine::ecs::PositionComponent>();

        auto &oldPos = sparseArray[id.entityId];
        oldPos->pos_x = pos.x;
        oldPos->pos_y = pos.y;
    } catch (std::exception &) {}
}

void rtype::client::Client::modifyVelocity(rtype::packets::Serialization::deserialize_archive &ar) {
    packets::EntityIdOnly id;
    packets::Vector2D vel;
    ar & id;
    ar & vel;
    try {
        auto &sparseArray = _registery.componentManager.getComponents<fengine::ecs::VelocityComponent>();

        auto &oldVel = sparseArray[id.entityId];
        oldVel->vel_x = vel.x;
        oldVel->vel_y = vel.y;
    } catch (std::exception &) {}
}
