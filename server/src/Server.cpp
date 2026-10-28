#include "Server.hpp"
#include "ClientHandler.hpp"
#include "TickHandler.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/BehaviourComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/event/PlayerEvent.hpp"
#include "r-type/systems/BehaviouralSystem.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <f-engine/ecs/Entity.hpp>
#include <f-engine/ecs/components/BoundaryComponent.hpp>
#include <f-engine/ecs/components/ControllableComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/components/VelocityComponent.hpp>
#include <f-engine/ecs/systems/BoundarySystem.hpp>
#include <f-engine/ecs/systems/ControlSystem.hpp>
#include <f-engine/ecs/systems/MoveSystem.hpp>
#include <iostream>
#include <optional>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/PacketBuilder.hpp>
#include <r-type/packets/Payload.hpp>
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
      _world(1.0, 10000), _newClientQueue(),
      // Handlers
      _tickHandler(), _clientHandler(generateClientQueue(), _newClientQueue, _packetBuilder),
      _subscriberHandler(_clientHandler, _packetBuilder),
      // Protocols
      _protocol(_io_context, _port, _tickHandler.getMessageQueue(), _clientHandler, _packetBuilder),
      _packetBuilder(_tickHandler),
      // Enemy generation
      _spawnClock(std::chrono::steady_clock::now()), _enemyCounter(0), _resendClock() {

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
    _world._registry.componentManager.registerComponent<fengine::ecs::CollisionComponent>();

    _world._registry.systemManager.addSystem<fengine::ecs::MovementSystem>(_world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::BoundarySystem>();
    _world._registry.systemManager.addSystem<fengine::ecs::BehaviouralSystem>();

    _subscriberHandler.add(_world._registry);
}

rtype::server::Server::~Server() {
}

std::queue<std::size_t> rtype::server::Server::generateClientQueue() {
    std::queue<std::size_t> clientQueue;

    for (std::size_t i = 0; i < MAX_PLAYERS; i++) {
        clientQueue.push(_world._registry.entityManager.spawnEntity());
    }
    return clientQueue;
}

void rtype::server::Server::newEntityPacket(std::size_t entityId, std::size_t, std::string uuid) {
    auto &velocityComponents = _world._registry.componentManager.getComponents<fengine::ecs::VelocityComponent>();
    auto &positionComponents = _world._registry.componentManager.getComponents<fengine::ecs::PositionComponent>();
    // auto &boundaryComponents =
    //     _world._registry.componentManager.getComponents<fengine::ecs::BoundaryComponent>();
    // auto &attackComponents = _world._registry.componentManager.getComponents<fengine::ecs::AttackComponent>();
    // auto &defenseComponents = _world._registry.componentManager.getComponents<fengine::ecs::DefenseComponent>();

    // Send a new entity
    {
        packets::EntitySpawn payload = {.entityId = entityId};
        std::string content =
            _packetBuilder.create(packets::PacketType::EntitySpawn).addHeader().addPayload(payload).get();

        _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
    }

    // TODO: code cleaning would be appreciated here
    if (velocityComponents.containsAndHasValue(entityId)) {
        auto &velocity = velocityComponents[entityId];

        {
            packets::EntityVelocityChange payload = {
                .entityId = entityId,
                .velocity = packets::Vector2D(velocity->vel_x, velocity->vel_y),
            };

            std::string content =
                _packetBuilder.create(packets::PacketType::EntityVelocityChange).addHeader().addPayload(payload).get();

            _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
        }
    }
    if (positionComponents.containsAndHasValue(entityId)) {
        auto &position = positionComponents[entityId];

        {
            packets::EntityMove payload = {
                .entityId = entityId,
                .coordinates = packets::Vector2D(position->pos_x, position->pos_y),
            };

            std::string content =
                _packetBuilder.create(packets::PacketType::EntityMove).addHeader().addPayload(payload).get();

            _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
        }
    }
}

void rtype::server::Server::userLoginPacket(std::size_t tick, std::string uuid) {
    const std::vector<std::optional<fengine::ecs::Entity>> &entities = _world._registry.entityManager.getEntities();

    std::cout << "user login packet tick " << tick << std::endl;
    for (auto &entity : entities) {
        const std::size_t entityId = entity->getId();
        newEntityPacket(entityId, tick, uuid);
    }
}

void rtype::server::Server::interpretPacket(packets::Serialization::deserialize_archive &ar,
    packets::PacketHeader &header) {
    switch (header.type) {
        case packets::PacketType::Ack: {
            std::string uuid(header.playerUuid);
            _clientHandler.setAlive(uuid);
            break;
        }
        case packets::PacketType::LoginRequest: {
            userLoginPacket(_tickHandler.getTick(), header.playerUuid);
            break;
        }
        case rtype::packets::PacketType::PlayerEvent: {
            handlePlayerEvents(ar, header);
            break;
        }
        default:
            break;
    }
}

void rtype::server::Server::handlePlayerEvents(packets::Serialization::deserialize_archive &ar,
    packets::PacketHeader &header) {
    // TODO: all types
    packets::PlayerEvent event;

    packets::Serialization::deserialize(ar, event);
    auto &data = _clientHandler.getClients().at(header.playerUuid);
    auto &vel = _world._registry.componentManager.getComponents<fengine::ecs::VelocityComponent>()[data.entityId];

    if (!vel.has_value())
        return;

    switch (event.event) {
        case event::Event::UP:
            handleVelocityChanges(vel.value().vel_y, vel.value().vel_x, -10);
            break;
        case event::Event::LEFT:
            handleVelocityChanges(vel.value().vel_x, vel.value().vel_y, -10);
            break;
        case event::Event::DOWN:
            handleVelocityChanges(vel.value().vel_y, vel.value().vel_x, 10);
            break;
        case event::Event::RIGHT:
            handleVelocityChanges(vel.value().vel_x, vel.value().vel_y, 10);
            break;
        case event::Event::SHOOT:
            fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();
            fengine::ecs::PositionComponent &pos =
                _world._registry.componentManager.getComponents<fengine::ecs::PositionComponent>()[data.entityId]
                    .value();
            fengine::ecs::AttackComponent &att =
                _world._registry.componentManager.getComponents<fengine::ecs::AttackComponent>()[data.entityId].value();
            _world._registry.componentManager.addComponent(entity,
                (fengine::ecs::PositionComponent) {pos.pos_x, pos.pos_y, true});
            _world._registry.componentManager.addComponent(entity, (fengine::ecs::VelocityComponent) {30, 0, false});
            _world._registry.componentManager.addComponent(entity,
                (fengine::ecs::BoundaryComponent) {10, 5, false, true});
            _world._registry.componentManager.addComponent(entity,
                (fengine::ecs::AttackComponent) {{.team = fengine::ecs::team::ALLIES}, att.damage});
            _world._registry.componentManager.addComponent(entity, (fengine::ecs::DefenseComponent) {1, 0});

            break;
    }
}

void rtype::server::Server::handleVelocityChanges(float &velocity, float counterVelocity, float value) {
    if (counterVelocity) {
        velocity = value / 2;
    } else {
        velocity = value;
    }
}

void rtype::server::Server::generateEnemies() {
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> spawnTime = elapsedTime - _spawnClock;
    std::chrono::milliseconds var(2000);

    if (spawnTime < var)
        return;

    fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();

    fengine::ecs::PositionComponent pos = {WINDOW_X, static_cast<float>(rand() % (WINDOW_Y - 100) + 100), false};
    fengine::ecs::VelocityComponent vel = {-10, 0, false};
    fengine::ecs::BoundaryComponent bound = {125, 75, false, true, std::nullopt};
    fengine::ecs::AttackComponent atk = {{.team = fengine::ecs::team::MONSTERS}, 10};
    fengine::ecs::DefenseComponent def = {150, 0};

    if (_enemyCounter == 10) {
        _enemyCounter = 0;
        vel = {-5, 2, false};
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
        newEntityPacket(entityId, _tickHandler.getTick(), client.first);
    }

    _spawnClock = std::chrono::steady_clock::now();
}

void rtype::server::Server::checkResend() {
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> spawnTime = elapsedTime - _resendClock;
    std::chrono::seconds var(2);

    if (spawnTime < var) {
        return;
    }

    auto &clients = _clientHandler.getClients();
    auto &entities = _world._registry.entityManager.getEntities();

    for (auto &entity : entities) {
        const std::size_t entityId = entity->getId();
        for (auto &client : clients) {
            newEntityPacket(entityId, _tickHandler.getTick(), client.first);
        }
    }
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
        while (!_newClientQueue.empty()) {
            std::size_t entityId = _newClientQueue.pop();
            std::optional<fengine::ecs::Entity> &entity = _world._registry.entityManager.entityFromIndex(entityId);

            fengine::ecs::PositionComponent pos = {50, 50, false};
            fengine::ecs::VelocityComponent vel = {0, 0, true};
            fengine::ecs::AttackComponent att = {{.team = fengine::ecs::team::ALLIES}, 10};
            fengine::ecs::DefenseComponent def = {100, 0};

            _world._registry.componentManager.addComponent(entity.value(), std::move(pos));
            _world._registry.componentManager.addComponent(entity.value(), std::move(vel));
            _world._registry.componentManager.addComponent(entity.value(), std::move(att));
            _world._registry.componentManager.addComponent(entity.value(), std::move(def));
        }
        while (!_tickHandler.getMessageQueue().empty()) {
            std::string msg = _tickHandler.getMessageQueue().pop();

            packets::PacketHeader header;
            try {
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
        checkResend();
        _clientHandler.verifyStatus();
        _world._registry.systemManager.update(_world._registry);
        _subscriberHandler.process();
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _protocol.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });

    _threadPool.join();
}
