#include "Client.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/detail/error_code.hpp"
#include "f-engine/asset_manager/SFMLAssetManager.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/AnimationComponent.hpp"
#include "f-engine/ecs/components/ControllableComponent.hpp"
#include "f-engine/ecs/components/DeadComponent.hpp"
#include "f-engine/ecs/components/DrawableComponent.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "f-engine/ecs/components/VelocityComponent.hpp"
#include "f-engine/ecs/systems/AnimationSystem.hpp"
#include "f-engine/ecs/systems/DisplaySystem.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventMouseButton.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "f-engine/modules/display/shapes/SFMLRectangle.hpp"
#include "f-engine/modules/sound/SFMLSoundModule.hpp"
#include "r-type/components/EntityTypeComponent.hpp"
#include "r-type/config/ClientConfig.hpp"
#include "r-type/event/PlayerEvent.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Payload.hpp"
#include "r-type/packets/Serialization.hpp"
#include "r-type/presets/EntitiesPresets.hpp"
#include "r-type/scene/GameScene.hpp"
#include "r-type/scene/MenuScene.hpp"
#include "r-type/scene/ParamScene.hpp"
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
    : _assetManager(std::make_shared<fengine::asset_manager::SFMLAssetManager>()), _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>(std::static_pointer_cast<fengine::asset_manager::SFMLAssetManager>(_assetManager))),
      _soundModule(std::make_unique<fengine::modules::sound::SFMLSoundModule>(std::static_pointer_cast<fengine::asset_manager::SFMLAssetManager>(_assetManager))), _registery(), _camera(0.0, 100000),
      _scenes(), _currentScene(), _writeThread(), _readThread(), _exit(false), _inputs(1024),
      _protocol(_inputs, host, port, _maxTick), _packetReceived(), _maxTick(0) {

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
    _registery.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _registery.componentManager.registerComponent<fengine::ecs::EntityTypeComponent>();

    fengine::ecs::ControllableComponent control = {fengine::modules::display::EventKey::Z,
        fengine::modules::display::EventKey::Q, fengine::modules::display::EventKey::S,
        fengine::modules::display::EventKey::D};

    _registery.systemManager.addSystem<fengine::ecs::DisplaySystem>(*_displayModule, _camera);
    _registery.systemManager.addSystem<fengine::ecs::AnimationSystem>(*_displayModule, _camera);
    _soundModule->playMusic("./assets/sounds/title.ogg");
    _clientConfig->set("controls", fengine::ecs::ControllableComponent(control));
    _clientConfig->resetChange();
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

        switch (header.type) {
            // TODO: All packets
            case rtype::packets::PacketType::Ack:
                break;
            case rtype::packets::PacketType::PlayerEvent:
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

        applyReceivedPackets();

        _displayModule->drawScene(_currentScene);
        _registery.systemManager.update(_registery);

        std::optional<std::queue<std::unique_ptr<fengine::modules::display::IEvent>>> events =
            _displayModule->pollEvent();
        if (events.has_value()) {
            while (events.value().empty() == false) {
                std::unique_ptr<fengine::modules::display::IEvent> event = std::move(events.value().front());
                events.value().pop();
                if (event->getType() == fengine::modules::display::EventType::Quit) {
                    _exit = true;
                    break;
                }
                if (event->getType() == fengine::modules::display::EventType::KeyPressed) {
                    auto control = _clientConfig->get("controls");
                    auto key = event->getKey();

                    if (control->has_value()) {
                        auto value = std::any_cast<fengine::ecs::ControllableComponent>(control.value());
                        if (key == value.up)
                            _inputs.addElement(event::Event::UP);
                        else if (key == value.down)
                            _inputs.addElement(event::Event::DOWN);
                        else if (key == value.left)
                            _inputs.addElement(event::Event::LEFT);
                        else if (key == value.right)
                            _inputs.addElement(event::Event::RIGHT);
                    }
                }
                if (event->getType() == fengine::modules::display::EventType::Click) {
                    if (event->getMouseButton() == fengine::modules::display::EventMouseButton::Left)
                        _inputs.addElement(event::Event::SHOOT);
                    _currentScene->updateButtons(event->getClickCoordinates());
                }
            }
            if (_exit == true) {
                break;
            }
        }
        while (!_currentScene->empty()) {
            interpretButtonAction(_currentScene->pollEvents());
            _displayModule->verifyConfig();
        }
        _currentScene->update();
    }
}

void rtype::client::Client::applyReceivedPackets() {
    while (_packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.size() > 0) {
        interpretMsg(_packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.front());
        _packetReceived[(_maxTick - INTERPOLATION_TICK) % 10].second.pop();
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
        static_cast<void>(_);
    } catch (std::exception &) {
        fengine::ecs::Entity entity = _registery.entityManager.createEntity(payload.entityId);
        struct fengine::presets::presets preset = fengine::presets::EntityPresets[static_cast<int>(payload.entityType)];

        _registery.componentManager.addComponent(entity, (fengine::ecs::EntityTypeComponent) {payload.entityType});
        _registery.componentManager.addComponent(entity, (fengine::ecs::PositionComponent) {200, 200, false});
        _registery.componentManager.addComponent(entity, (fengine::ecs::VelocityComponent) {0, 0, false});
        _registery.componentManager.addComponent(entity,
            (fengine::ecs::DrawableComponent) {std::string(preset.preset.texture),
                std::make_shared<fengine::modules::display::SFMLRectangle>(payload.width, payload.height),
                std::nullopt});
        _registery.componentManager.addComponent(entity,
            (fengine::ecs::AnimationComponent) {preset.values.length, preset.values.height, preset.values.offset_x,
                preset.values.offset_y, preset.values.cols, preset.values.rows, false,
                std::chrono::steady_clock::now()});
        _soundModule->playSound(std::string(preset.preset.apparition_sound));
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
