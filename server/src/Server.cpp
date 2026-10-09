#include "Server.hpp"
#include <cstdint>
#include <iostream>

rtype::server::Server::Server(std::uint32_t port)
    : _port(port), _io_context(),
      // Threads
      // For now, we're assuming the threads amount ourselves.
      _totalThreadAmount(4), _protocolThreadAmount(3), _threadPool(_totalThreadAmount),
      // Tick handling
      _tickHandler(),
      // Protocols
      _udp(_io_context, _port, _tickHandler.getMessageQueue()) {
}

rtype::server::Server::~Server() {
}

void rtype::server::Server::tickThreadRun() {
    while (true) {
        if (!_tickHandler.tickPassed())
            continue;
        while (!_tickHandler.getMessageQueue().empty()) {
            std::string msg = _tickHandler.getMessageQueue().pop();
            std::cout << "Got message: " << msg << std::endl;
        }
    }
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _protocolThreadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    boost::asio::post(_threadPool, [&]() { this->tickThreadRun(); });
    _threadPool.join();
}
