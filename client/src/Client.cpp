#include "Client.hpp"
#include "boost/asio/error.hpp"
#include "boost/exception/exception.hpp"
#include "boost/system/detail/error_code.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/BoundaryComponent.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/systems/ControlSystem.hpp"
#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "f-engine/modules/display/shapes/SFMLCircle.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include "f-engine/modules/sound/SFMLSoundModule.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/config/ClientConfig.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Payload.hpp"
#include "r-type/packets/Serialization.hpp"
#include "r-type/presets/EntitiesPresets.hpp"
#include "r-type/scene/GameScene.hpp"
#include "r-type/scene/MenuScene.hpp"
#include "r-type/scene/ParamScene.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

rtype::client::Client::Client(std::string host, std::string port)
    : _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()),
      _soundModule(std::make_unique<fengine::modules::sound::SFMLSoundModule>()), _registery(), _camera(0.0, 100000),
      _scenes(), _currentScene(), _entity(_registery.entityManager.spawnEntity()), _writeThread(), _readThread(),
      _exit(false), _inputs(1024), _protocol(_inputs, host, port), _packetReceived(), _maxTick(0) {

    _clientConfig = std::make_shared<fengine::config::ClientConfig>();
    _scenes.push_back(std::make_shared<fengine::scene::MenuScene>("./assets/test.png", false));
    _scenes.push_back(std::make_shared<fengine::scene::ParamScene>("./assets/test.png", false));
    _scenes.push_back(std::make_shared<fengine::scene::GameScene>(3.0, "./assets/test.png", true));
    _currentScene = _scenes.front();
    _displayModule->setConfig(_clientConfig);
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

    _registery.systemManager.addSystem<fengine::ecs::ControlSystem>(_pressqueue, _releasequeue);
    _registery.systemManager.addSystem<fengine::ecs::DisplaySystem>(*_displayModule, _camera);
    _soundModule->playMusic("./assets/sounds/title.ogg");
}

rtype::client::Client::~Client() {
    _writeThread.join();
    _readThread.join();
}

void rtype::client::Client::run() {
    _writeThread = std::thread(&rtype::client::Client::sendLoop, this);
    _readThread = std::thread(&rtype::client::Client::readLoop, this);
    loop();
}

void rtype::client::Client::sendLoop() {
    while (!_exit) {
        _protocol.sendMessage();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "BYE BYE" << std::endl;
}

void rtype::client::Client::readLoop() {
    try {
        _protocol.setSocketNonBlocking();
        while (!_exit) {
            boost::system::error_code error = boost::asio::error::would_block;
            while (error == boost::asio::error::would_block && !_exit) {
                std::string msg = _protocol.read(error);
                stockMsg(msg);
            }
        }
    } catch (std::exception &) {}
}

void rtype::client::Client::stockMsg(std::string msg) {
    packets::PacketHeader header;

    try {
        std::stringstream ss(msg);
        packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);
        packets::Serialization::deserialize(ar, header);

        // TODO handle overflow
        if (_maxTick < header.tick) {
            _maxTick = header.tick;
        }
        if (_packetReceived[header.tick % 10].first == header.tick) {
            _packetReceived[header.tick % 10].second.push(msg);
        } else {
            _packetReceived[header.tick % 10].first = header.tick;

            while (!_packetReceived[header.tick % 10].second.empty()) {
                _packetReceived[header.tick % 10].second.pop();
            }
            _packetReceived[header.tick % 10].second.push(msg);
        }
    } catch (std::exception &) {}
}

void rtype::client::Client::interpretMsg(std::string msg) {
    packets::PacketHeader header;

    try {
        std::stringstream ss(msg);
        packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);
        packets::Serialization::deserialize(ar, header);

        // TODO
        // when new tick is receive I should get the content of the previous tick and then apply the modifications
        // all this data is stocked in an ordered map to witch we acces any element with the tick id as the key
        // at each client frame I should do a kind of calculus to know if I need to pass to the next tick or not the server has 20 ticks per seconds but the client has 60 fps

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
    // at the connection the client will wait a bit before starting the display
    // it will wait until the _snapshotVector is partially fill
    // when it is fill it will start display with some ticks of difference

    while (!_exit) {
        _displayModule->clear();

        while (_packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.size() > 0) {
            interpretMsg(_packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.front());
            _packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.pop();
        }

        _displayModule->drawScene(_currentScene);
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
            if (event.value()->getType() == fengine::modules::display::EventType::Click) {
                (_currentScene)->updateButtons(event.value()->getClickCoordinates());
            }
        }
        while (!_currentScene->empty()) {
            interpretButtonAction(_currentScene->pollEvents());
            _displayModule->verifyConfig();
        }
        _currentScene->update();
    }
}

void rtype::client::Client::removeEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntityDied payload;

    packets::Serialization::deserialize(ar, payload);

    try {
        auto &entity = _registery.entityManager.getEntities().at(payload.entityId);
        _registery.entityManager.killEntity(entity.value());
    } catch (std::exception &) {}
}

void rtype::client::Client::addEntity(packets::Serialization::deserialize_archive &ar) {
    packets::EntitySpawn payload;

    packets::Serialization::deserialize(ar, payload);

    try {
        auto &_ = _registery.entityManager.getEntities().at(payload.entityId);
    } catch (std::exception &) {
        fengine::ecs::Entity entity = _registery.entityManager.createEntity(payload.entityId);
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
        // _soundModule->playSound(EntityPresets[static_cast<int>(payload.ENtityEnum (exemple : BULLET))].preset.apparition_sound); commented because not implemented
    }
}

void rtype::client::Client::moveEntity(rtype::packets::Serialization::deserialize_archive &ar) {
    packets::EntityMove payload;

    packets::Serialization::deserialize(ar, payload);

    try {
        auto &sparseArray = _registery.componentManager.getComponents<fengine::ecs::PositionComponent>();

        auto &oldPos = sparseArray[payload.entityId];
        oldPos->pos_x = payload.coordinates.x;
        oldPos->pos_y = payload.coordinates.y;
    } catch (std::exception &) {}
}

void rtype::client::Client::modifyVelocity(rtype::packets::Serialization::deserialize_archive &ar) {
    packets::EntityVelocityChange payload;

    packets::Serialization::deserialize(ar, payload);

    try {
        auto &sparseArray = _registery.componentManager.getComponents<fengine::ecs::VelocityComponent>();

        auto &oldVel = sparseArray[payload.entityId];
        oldVel->vel_x = payload.velocity.x;
        oldVel->vel_y = payload.velocity.y;
    } catch (std::exception &) {}
}

void rtype::client::Client::interpretButtonAction(int button) {
    switch (button) {
        case static_cast<int>(fengine::scene::MenuSceneButtons::PLAY):
            _soundModule->playMusic("./assets/sounds/game.ogg");
            _currentScene = _scenes.back();
            break;
        case static_cast<int>(fengine::scene::MenuSceneButtons::PARAMS):
            _soundModule->playMusic("./assets/sounds/params.ogg");
            _currentScene = *(_scenes.begin() + 1);
            break;
        case static_cast<int>(fengine::scene::MenuSceneButtons::QUIT):
            _exit = true;
            break;
        case static_cast<int>(fengine::scene::ParamSceneButtons::MENU):
            _soundModule->playMusic("./assets/sounds/title.ogg");
            _currentScene = _scenes.front();
            break;
        case static_cast<int>(fengine::scene::ParamSceneButtons::COLORBLIND_1):
            _clientConfig->set("colorblind", std::pair<bool, int>(true, 1));
            break;
        case static_cast<int>(fengine::scene::ParamSceneButtons::COLORBLIND_2):
            _clientConfig->set("colorblind", std::pair<bool, int>(true, 2));
            break;
        case static_cast<int>(fengine::scene::ParamSceneButtons::COLORBLIND_3):
            _clientConfig->set("colorblind", std::pair<bool, int>(true, 3));
            break;
    }
    return;
}
