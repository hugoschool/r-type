#include "Server.hpp"
#include "ClientHandler.hpp"
#include "TickHandler.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/ecs/components/CollisionComponent.hpp"
#include "f-engine/modules/display/window/IWindow.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/BehaviourComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include "r-type/components/EntityTypeComponent.hpp"
#include "packets.pb.h"
#include "r-type/systems/BehaviouralSystem.hpp"
#include "r-type/systems/CollisionDetectionSystem.hpp"
#include "r-type/systems/CollisionProcessorSystem.hpp"
#include "r-type/systems/KillingSystem.hpp"
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
#include <r-type/packets/PacketBuilder.hpp>
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
    _world._registry.componentManager.registerComponent<fengine::ecs::EntityTypeComponent>();

    _world._registry.systemManager.addSystem<fengine::ecs::MovementSystem>(_world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::CollisionDetectionSystem>();
    _world._registry.systemManager.addSystem<fengine::ecs::CollisionProcessorSystem>();
    _world._registry.systemManager.addSystem<fengine::ecs::KillingSystem>();
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

void rtype::server::Server::newEntityPacket(std::size_t entityId, rtype::packets::EntityType entityType, std::size_t,
    std::string uuid) {
    auto &velocityComponents = _world._registry.componentManager.getComponents<fengine::ecs::VelocityComponent>();
    auto &positionComponents = _world._registry.componentManager.getComponents<fengine::ecs::PositionComponent>();
    auto &boundaryComponents = _world._registry.componentManager.getComponents<fengine::ecs::BoundaryComponent>();

    // Send a new entity
    if (boundaryComponents.containsAndHasValue(entityId)) {
        auto &boundary = boundaryComponents[entityId];

        {
            auto *payload = new packets::EntitySpawn();
            payload->set_entityid(entityId);
            payload->set_entitytype(entityType);
            payload->set_width(boundary->size_x);
            payload->set_height(boundary->size_y);

            std::string content =
                _packetBuilder.create(packets::TypeEntitySpawn).addHeader().addPayload(payload).get();

            _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
        }
    }

    // TODO: code cleaning would be appreciated here
    if (velocityComponents.containsAndHasValue(entityId)) {
        auto &velocity = velocityComponents[entityId];

        {
            auto *payload = new packets::EntityVelocityChange();
            payload->set_entityid(entityId);

            auto *allocatedVelocity = new packets::Vector2D();
            allocatedVelocity->set_x(velocity->vel_x);
            allocatedVelocity->set_y(velocity->vel_y);

            payload->set_allocated_velocity(allocatedVelocity);

            std::string content =
                _packetBuilder.create(packets::TypeEntityVelocityChange).addHeader().addPayload(payload).get();

            _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
        }
    }
    if (positionComponents.containsAndHasValue(entityId)) {
        auto &position = positionComponents[entityId];

        {
            auto *payload = new packets::EntityMove();
            payload->set_entityid(entityId);

            auto *allocatedCoordinates = new packets::Vector2D();
            allocatedCoordinates->set_x(position->pos_x);
            allocatedCoordinates->set_y(position->pos_y);

            payload->set_allocated_coordinates(allocatedCoordinates);

            std::string content = _packetBuilder.create(packets::TypeEntityMove).addHeader().addPayload(payload).get();

            _clientHandler.getMessageQueueHandler().addMessage(uuid, content);
        }
    }
}

void rtype::server::Server::userLoginPacket(std::size_t tick, std::string uuid) {
    const std::vector<std::optional<fengine::ecs::Entity>> &entities = _world._registry.entityManager.getEntities();
    auto &typeComponents = _world._registry.componentManager.getComponents<fengine::ecs::EntityTypeComponent>();

    std::cout << "user login packet tick " << tick << std::endl;
    for (auto &entity : entities) {
        if (!entity.has_value())
            continue;
        const std::size_t entityId = entity->getId();

        if (typeComponents[entityId].has_value()) {
            newEntityPacket(entityId, typeComponents[entityId].value().type, tick, uuid);
            continue;
        }
    }
}

void rtype::server::Server::interpretPacket(packets::Packet &packet) {
    const packets::PacketHeader &header = packet.header();

    switch (header.type()) {
        case packets::TypeAck: {
            std::string uuid(header.playeruuid());
            _clientHandler.setAlive(uuid);
            break;
        }
        case packets::TypeLoginRequest: {
            userLoginPacket(_tickHandler.getTick(), header.playeruuid());
            break;
        }
        case packets::TypePlayerEvent: {
            handlePlayerEvents(packet);
            break;
        }
        default:
            break;
    }
}

void rtype::server::Server::handlePlayerEvents(packets::Packet &packet) {
    const packets::PacketHeader &header = packet.header();
    const packets::PlayerEvent &event = packet.playerevent();

    auto &data = _clientHandler.getClients().at(header.playeruuid());
    auto &entity = _world._registry.entityManager.entityFromIndex(data.entityId);

    if (!entity.has_value())
        return;

    auto &vel = _world._registry.componentManager.getComponents<fengine::ecs::VelocityComponent>()[data.entityId];

    if (!vel.has_value())
        return;

    switch (event.event()) {
        case packets::EVENT_UP:
            handleVelocityChanges(vel.value().vel_y, vel.value().vel_x, -10);
            break;
        case packets::EVENT_LEFT:
            handleVelocityChanges(vel.value().vel_x, vel.value().vel_y, -10);
            break;
        case packets::EVENT_DOWN:
            handleVelocityChanges(vel.value().vel_y, vel.value().vel_x, 10);
            break;
        case packets::EVENT_RIGHT:
            handleVelocityChanges(vel.value().vel_x, vel.value().vel_y, 10);
            break;
        case packets::EVENT_SHOOT: {
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
                (fengine::ecs::BoundaryComponent) {10, 5, false, true, std::nullopt});
            _world._registry.componentManager.addComponent(entity,
                (fengine::ecs::AttackComponent) {{.team = fengine::ecs::team::ALLIES}, att.damage});
            _world._registry.componentManager.addComponent(entity, (fengine::ecs::DefenseComponent) {1, 0});
            _world._registry.componentManager.addComponent(entity,
                (fengine::ecs::EntityTypeComponent) {packets::ENTITY_BULLETS});

            break;
        }
        default:
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
    std::chrono::milliseconds var(4000);

    if (spawnTime < var)
        return;

    fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();

    fengine::ecs::PositionComponent pos = {WINDOW_X, static_cast<float>(rand() % (WINDOW_Y - 100) + 100), false};
    fengine::ecs::VelocityComponent vel = {-5, 0, false};
    fengine::ecs::BoundaryComponent bound = {100, 50, false, true, std::nullopt};
    fengine::ecs::AttackComponent atk = {{.team = fengine::ecs::team::MONSTERS}, 10};
    fengine::ecs::DefenseComponent def = {150, 0};

    rtype::packets::EntityType type = packets::ENTITY_SHOOTER;
    fengine::ecs::EntityTypeComponent typeComponent = {.type = type};

    if (_enemyCounter == 5) {
        _enemyCounter = 0;
        vel = {-5, 2, false};
        atk.damage += 15;
        def = {300, 20};
        bound.size_x += 100;
        bound.size_y += 50;
        bound.bouncy = true;
        type = rtype::packets::ENTITY_BOSS;
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
    _world._registry.componentManager.addComponent(entity, std::move(typeComponent));

    auto &clients = _clientHandler.getClients();
    const std::size_t entityId = entity.getId();
    for (auto &client : clients) {
        newEntityPacket(entityId, type, _tickHandler.getTick(), client.first);
    }

    _spawnClock = std::chrono::steady_clock::now();
}

void rtype::server::Server::checkResend() {
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> spawnTime = elapsedTime - _resendClock;
    std::chrono::milliseconds var(50);

    if (spawnTime < var) {
        return;
    }

    auto &clients = _clientHandler.getClients();
    auto &entities = _world._registry.entityManager.getEntities();
    auto &typeComponents = _world._registry.componentManager.getComponents<fengine::ecs::EntityTypeComponent>();

    for (auto &entity : entities) {
        if (!entity.has_value())
            continue;
        const std::size_t entityId = entity->getId();
        for (auto &client : clients) {
            if (typeComponents[entityId].has_value()) {
                newEntityPacket(entityId, typeComponents[entityId].value().type, _tickHandler.getTick(), client.first);
                continue;
            }
        }
    }

    _resendClock = elapsedTime;
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
            fengine::ecs::BoundaryComponent bound = {100, 50, false, true, std::nullopt};
            fengine::ecs::EntityTypeComponent type = {packets::ENTITY_PLAYERS};

            _world._registry.componentManager.addComponent(entity.value(), std::move(pos));
            _world._registry.componentManager.addComponent(entity.value(), std::move(vel));
            _world._registry.componentManager.addComponent(entity.value(), std::move(att));
            _world._registry.componentManager.addComponent(entity.value(), std::move(def));
            _world._registry.componentManager.addComponent(entity.value(), std::move(bound));
            _world._registry.componentManager.addComponent(entity.value(), std::move(type));
        }
        while (!_tickHandler.getMessageQueue().empty()) {
            std::string msg = _tickHandler.getMessageQueue().pop();

            packets::Packet packet;
            try {
                std::stringstream ss(msg);
                packets::Serialization::deserialize_archive &ar = packets::Serialization::createDeserializeArchive(ss);

                packets::Serialization::deserialize(ar, packet);
                interpretPacket(packet);
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
