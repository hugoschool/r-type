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
      // Tick handling
      _tickHandler(),
      // Clients
      _clientHandler(),
      // Engine
      _world(1.0, 10000),
      // Protocols
      _udp(_io_context, _port, _tickHandler.getMessageQueue(), _clientHandler.getMessageQueueHandler()) {

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

void rtype::server::Server::interpretPacket(packets::Serialization::deserialize_archive &ar,
    packets::PacketHeader &header) {
    switch (header.type) {
        case packets::PacketType::Ack: {
            std::string uuid = _clientHandler.generateUuid();
            _clientHandler.addClient(uuid);
            _clientHandler.setAlive(uuid);
            break;
        }
        // test
        case rtype::packets::PacketType::InputPlayer: {
            // Debug
            // rtype::packets::Input input;
            // packets::Serialization::deserialize(ar, input);

            // std::cout << "Input is " << static_cast<int>(input.input) << std::endl;

            // if (input.input == fengine::modules::display::EventKey::Z) {
            //     _clientHandler.getMessageQueueHandler().addMessage(header.playerUuid, "ZZzzzzzzzzzzzz");
            // }
            // if (input.input == fengine::modules::display::EventKey::A) {
            //     _clientHandler.getMessageQueueHandler().addMessage(header.playerUuid, "AAaaaaaaaaaaaa");
            // }
            // if (input.input == fengine::modules::display::EventKey::S) {
            //     _clientHandler.getMessageQueueHandler().addMessage(header.playerUuid, "SSssssssssssssssssss");
            // }
            // std::stringstream ss;
            // packets::PacketHeader NewHeader = {
            //     .type = packets::PacketType::Ack,
            //     .size = 0,
            //     // TODO: this is a simple test UUID for the server <-> client communication
            //     // To be removed later
            //     .playerUuid = "f52ea348-6c57-4acf-9834-059bc9a9c81c",
            // };

            // packets::Serialization::serialize(ss, NewHeader, packets::Empty{});

            // std::string send = ss.str();
            // _clientHandler.getMessageQueueHandler().addMessage(header.playerUuid, send);
        }
        default:
            break;
    }
}

void rtype::server::Server::tickThreadRun() {
    while (true) {
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
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });
    _threadPool.join();
}
