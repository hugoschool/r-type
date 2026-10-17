#include "Server.hpp"
#include <cstdint>
#include <f-engine/ecs/components/AttackComponent.hpp>
#include <f-engine/ecs/components/BoundaryComponent.hpp>
#include <f-engine/ecs/components/DefenseComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/components/VelocityComponent.hpp>
#include <f-engine/ecs/systems/BoundarySystem.hpp>
#include <f-engine/ecs/systems/MoveSystem.hpp>
#include <iostream>
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
      _udp(_io_context, _port, _tickHandler.getMessageQueue(), _clientHandler) {

    _signals.async_wait([&](boost::system::error_code, int) {
        _io_context.stop();
        _stop = true;
    });

    _world._registry.componentManager.registerComponent<fengine::ecs::PositionComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::VelocityComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::BoundaryComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::AttackComponent>();
    _world._registry.componentManager.registerComponent<fengine::ecs::DefenseComponent>();

    _world._registry.systemManager.addSystem<fengine::ecs::MovementSystem>(_world._camera);
    _world._registry.systemManager.addSystem<fengine::ecs::BoundarySystem>();

    _subscriberHandler.add(_world._registry);

    // TODO: temporary monster spawning & moving mechanics
    float posx = 0;
    float posy = 0;
    float vel_x = 8;
    float vel_y = 7;
    fengine::ecs::team team = fengine::ecs::team::MONSTERS;
    for (int i = 0; i < 5; i++) {
        fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();
        fengine::ecs::PositionComponent pos = {posx, posy, false};
        fengine::ecs::VelocityComponent vel = {vel_x, vel_y};
        fengine::ecs::BoundaryComponent bound = {10, 10, true, std::nullopt};
        fengine::ecs::AttackComponent att = {{.team = team}, 10};
        fengine::ecs::DefenseComponent def = {100, 0};

        _world._registry.componentManager.addComponent(entity, std::move(pos));
        _world._registry.componentManager.addComponent(entity, std::move(vel));
        _world._registry.componentManager.addComponent(entity, std::move(bound));
        _world._registry.componentManager.addComponent(entity, std::move(att));
        _world._registry.componentManager.addComponent(entity, std::move(def));
        posx += 0.1;
        posy += 0.1;
        if (vel_x > 0)
            vel_x += 0.1;
        vel_x *= -1;
        if (vel_y > 0)
            vel_y += 0.1;
        vel_y *= -1;
    }
}

rtype::server::Server::~Server() {
}

void rtype::server::Server::interpretPacket(packets::Serialization::deserialize_archive &,
    packets::PacketHeader &header) {
    switch (header.type) {
        case packets::PacketType::Ack: {
            std::string uuid(header.playerUuid);
            _clientHandler.setAlive(uuid);
            break;
        }
        // TODO: all types
        case rtype::packets::PacketType::InputPlayer: {
        }
        default:
            break;
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
