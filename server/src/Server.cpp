#include "Server.hpp"
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>
#include <cstdint>
#include <iostream>
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
      // Protocols
      _udp(_io_context, _port, _tickHandler.getMessageQueue()) {
}

rtype::server::Server::~Server() {
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
            std::cout << "Got message: " << msg << std::endl;

            std::string uuid = _clientHandler.generateUuid();
            _clientHandler.addClient(uuid);
            _clientHandler.setAlive(uuid);
        }
        _clientHandler.verifyStatus();
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });
    _threadPool.join();
}
