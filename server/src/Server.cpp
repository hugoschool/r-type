#include "Server.hpp"
#include "f-engine/modules/display/window/SFMLWindow.hpp"
#include <chrono>
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
#include <thread>

rtype::server::Server::Server(std::uint32_t port)
    : _port(port), _io_context(),
      // Threads
      // For now, we're assuming the threads amount ourselves.
      _totalThreadAmount(4), _protocolThreadAmount(3), _threadPool(_totalThreadAmount),
      // Signals handling
      _stop(false), _signals(_io_context, SIGINT, SIGTERM),
      // Tick handling
      _tickHandler(),
      // Clients
      _clientHandler(),
      // Engine
      _world(1.0, 10000),
      // Protocols
      _udp(_io_context, _port, _tickHandler.getMessageQueue(), _clientHandler.getMessageQueueHandler()),
      // Enemy generation
      _spawnClock(std::chrono::steady_clock::now()),
      _ennemyCounter(0) {

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

    // TODO: this is all temporary and to provide an easy message check to the client
    _clientHandler.getMessageQueueHandler().addMessage("f52ea348-6c57-4acf-9834-059bc9a9c81c", "Hello world!!!!!\n");
    _clientHandler.getMessageQueueHandler().addMessage("f52ea348-6c57-4acf-9834-059bc9a9c81c",
        "This is a simple test to verify connection between server & client.\n");
    _clientHandler.getMessageQueueHandler().addMessage("f52ea348-6c57-4acf-9834-059bc9a9c81c",
        "Let's hope this makes it through\n");
}

rtype::server::Server::~Server() {
}

void rtype::server::Server::interpretPacket(packets::Serialization::deserialize_archive &,
    packets::PacketHeader &header) {
    switch (header.type) {
        case packets::PacketType::Ack: {
            std::string uuid = _clientHandler.generateUuid();
            _clientHandler.addClient(uuid);
            _clientHandler.setAlive(uuid);
            break;
        }
        default:
            break;
    }
}

void rtype::server::Server::generateEnnemies() {
    std::chrono::steady_clock::time_point elapsedTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> spawnTime = elapsedTime - _spawnClock;
    std::chrono::milliseconds var(10000);

    if (spawnTime < var)
        return;

    fengine::ecs::PositionComponent pos = {WINDOW_X, static_cast<float>(rand() % (WINDOW_Y - 100) + 100), false};
    fengine::ecs::VelocityComponent vel = {-10, 0};
    fengine::ecs::BoundaryComponent bound = {125, 75, false, true};
    fengine::ecs::AttackComponent atk = {{.team = fengine::ecs::team::MONSTERS}, 10};
    fengine::ecs::DefenseComponent def = {150, 0};

    if (_ennemyCounter == 10) {
        _ennemyCounter = 0;
        vel = {-5, 2};
        atk.damage += 15;
        def = {300, 20};
        bound.size_x += 100;
        bound.size_y += 50;
        bound.bouncy = true;
    } else {
        _ennemyCounter++;
    }

    // envoyer un message de creation ici - vasy hugo, c'est pour toi
    // le client est grand et lui attribuera lui meme le drawable et le anim
    // ou alors on le met la et on l'envoie
    fengine::ecs::Entity entity = _world._registry.entityManager.spawnEntity();
    _world._registry.componentManager.addComponent(entity, std::move(pos));
    _world._registry.componentManager.addComponent(entity, std::move(vel));
    _world._registry.componentManager.addComponent(entity, std::move(bound));
    _world._registry.componentManager.addComponent(entity, std::move(atk));
    _world._registry.componentManager.addComponent(entity, std::move(def));
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
        generateEnnemies();
        _clientHandler.verifyStatus();
        _world._registry.systemManager.update(_world._registry);
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });

    _threadPool.join();
}
