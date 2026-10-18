#include "Server.hpp"
#include "f-engine/ecs/components/BehaviourComponent.hpp"
#include "f-engine/ecs/systems/BehaviouralSystem.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include <chrono>
#include <cstdint>
#include <f-engine/ecs/Entity.hpp>
#include <f-engine/ecs/components/AttackComponent.hpp>
#include <f-engine/ecs/components/BoundaryComponent.hpp>
#include <f-engine/ecs/components/DefenseComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/components/VelocityComponent.hpp>
#include <f-engine/ecs/systems/BoundarySystem.hpp>
#include <f-engine/ecs/systems/MoveSystem.hpp>
#include <iostream>
#include <optional>
#include <r-type/packets/Entity.hpp>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>
#include <sstream>
#include <thread>

rtype::server::Server::Server(std::uint32_t port)
    : _port(port), _io_context(),
      // Threads
      // For now, we're assuming the threads amount ourselves.
      _totalThreadAmount(4), _protocolThreadAmount(3), _threadPool(_totalThreadAmount),
      // Signals handling
      _stop(false), _signals(_io_context, SIGINT, SIGTERM),
      // Engine
      _subscriberHandler(_clientHandler), _world(1.0, 10000),
      // Handlers
      _tickHandler(), _clientHandler(_world._registry),
      // Protocols
      _udp(_io_context, _port, _tickHandler.getMessageQueue(), _clientHandler),
      // Enemy generation
      _spawnClock(std::chrono::steady_clock::now()), _enemyCounter(0) {

    _signals.async_wait([&](boost::system::error_code, int) {
        _io_context.stop();
        _stop = true;
    });

    _world._registry.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::AttackComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::DefenseComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::BehaviourComponent>();

    _world._registry.systemManager.addSystem<fengine::ecs::MovementSystem>(_world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::BoundarySystem>();
    _world._registry.systemManager.addSystem<fengine::ecs::BehaviouralSystem>();

    _subscriberHandler.add(_world._registry);
}

rtype::server::Server::~Server() {
}

void rtype::server::Server::newEntityPacket(std::size_t entityId, std::string uuid) {
    auto &velocityComponents = _world._registry.componentManager.getComponents<fengine::ecs::VelocityComponent>();
    auto &positionComponents = _world._registry.componentManager.getComponents<fengine::ecs::PositionComponent>();
    // auto &boundaryComponents =
    //     _world._registry.componentManager.getComponents<fengine::ecs::BoundaryComponent>();
    // auto &attackComponents = _world._registry.componentManager.getComponents<fengine::ecs::AttackComponent>();
    // auto &defenseComponents = _world._registry.componentManager.getComponents<fengine::ecs::DefenseComponent>();

    // Send a new entity
    {
        packets::EntitySpawn payload = {.entityId = entityId};

        packets::PacketHeader newHeader = {
            .type = packets::PacketType::EntitySpawn,
            .size = sizeof(payload),
            .playerUuid = "",
        };
        std::strncpy(newHeader.playerUuid, uuid.c_str(), packets::UUID_SIZE);

        std::stringstream ss;
        packets::Serialization::serialize(ss, newHeader, payload);
        _clientHandler.getMessageQueueHandler().addMessage(uuid, ss.str());
    }

    // TODO: code cleaning would be appreciated here
    if (velocityComponents.contains(entityId)) {
        auto &velocity = velocityComponents[entityId];

        {
            packets::EntityVelocityChange payload = {
                .entityId = entityId,
                .velocity = packets::Vector2D(velocity->vel_x, velocity->vel_y),
            };

            packets::PacketHeader newHeader = {
                .type = packets::PacketType::EntityVelocityChange,
                .size = sizeof(payload),
                .playerUuid = "",
            };
            std::strncpy(newHeader.playerUuid, uuid.c_str(), packets::UUID_SIZE);

            std::stringstream ss;
            packets::Serialization::serialize(ss, newHeader, payload);
            _clientHandler.getMessageQueueHandler().addMessage(uuid, ss.str());
        }
    }
    if (positionComponents.contains(entityId)) {
        auto &position = positionComponents[entityId];

        {
            packets::EntityMove payload = {
                .entityId = entityId,
                .coordinates = packets::Vector2D(position->pos_x, position->pos_y),
            };

            packets::PacketHeader newHeader = {
                .type = packets::PacketType::EntityVelocityChange,
                .size = sizeof(payload),
                .playerUuid = "",
            };
            std::strncpy(newHeader.playerUuid, uuid.c_str(), packets::UUID_SIZE);

            std::stringstream ss;
            packets::Serialization::serialize(ss, newHeader, payload);
            _clientHandler.getMessageQueueHandler().addMessage(uuid, ss.str());
        }
    }
}

void rtype::server::Server::userLoginPacket(std::string uuid) {
    const std::vector<std::optional<fengine::ecs::Entity>> &entities = _world._registry.entityManager.getEntities();

    for (auto &entity : entities) {
        const std::size_t entityId = entity->getId();
        newEntityPacket(entityId, uuid);
    }
}

void rtype::server::Server::interpretPacket(packets::Serialization::deserialize_archive &,
    packets::PacketHeader &header) {
    switch (header.type) {
        case packets::PacketType::Ack: {
            std::string uuid(header.playerUuid);
            _clientHandler.setAlive(uuid);
            break;
        }
        case packets::PacketType::LoginRequest: {
            userLoginPacket(header.playerUuid);
            break;
        }
        // TODO: all types
        case rtype::packets::PacketType::InputPlayer: {
            break;
        }
        default:
            break;
    }
}

void rtype::server::Server::generateEnemies() {
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> spawnTime = elapsedTime - _spawnClock;
    std::chrono::milliseconds var(10000);

    if (spawnTime < var)
        return;

    fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();

    fengine::ecs::PositionComponent pos = {WINDOW_X, static_cast<float>(rand() % (WINDOW_Y - 100) + 100), false};
    fengine::ecs::VelocityComponent vel = {-10, 0};
    fengine::ecs::BoundaryComponent bound = {125, 75, false, true, std::nullopt};
    fengine::ecs::AttackComponent atk = {{.team = fengine::ecs::team::MONSTERS}, 10};
    fengine::ecs::DefenseComponent def = {150, 0};

    if (_enemyCounter == 10) {
        _enemyCounter = 0;
        vel = {-5, 2};
        atk.damage += 15;
        def = {300, 20};
        bound.size_x += 100;
        bound.size_y += 50;
        bound.bouncy = true;
    } else {
        // fengine::ecs::BehaviourComponent behaviour = {fengine::ecs::behaviour::SHOOTER, std::chrono::steady_clock::now()};
        // _world._registry.componentManager.addComponent(entity, std::move(behaviour));
        _enemyCounter++;
    }

    _world._registry.componentManager.addComponent(entity, std::move(pos));
    _world._registry.componentManager.addComponent(entity, std::move(vel));
    _world._registry.componentManager.addComponent(entity, std::move(bound));
    _world._registry.componentManager.addComponent(entity, std::move(atk));
    _world._registry.componentManager.addComponent(entity, std::move(def));

    auto &clients = _clientHandler.getClients();
    const std::size_t entityId = entity.getId();
    for (auto &client : clients) {
        newEntityPacket(entityId, client.first);
    }

    _spawnClock = std::chrono::steady_clock::now();
}

void rtype::server::Server::tickThreadRun() {
    while (true) {
        if (_stop)
            return;
        if (!_tickHandler.tickPassed()) {
            // Using 100% CPU isn't wise, let's wait a little...
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        while (!_tickHandler.getMessageQueue().empty()) {
            std::string msg = _tickHandler.getMessageQueue().pop();

            packets::PacketHeader header;
            try {
                std::cout << "Got message: " << msg << std::endl;
                std::stringstream ss(msg);
                packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);

                packets::Serialization::deserialize(ar, header);
                interpretPacket(ar, header);
            } catch (std::exception &) {
                std::cerr << "Doesn't seem to be a packet..." << std::endl;
                continue;
            }
        }
        generateEnemies();
        _clientHandler.verifyStatus();
        _world._registry.systemManager.update(_world._registry);
        _subscriberHandler.process();
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });

    _threadPool.join();
}
