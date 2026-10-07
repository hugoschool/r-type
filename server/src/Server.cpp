#include "Server.hpp"
#include <cstdint>
#include <thread>

rtype::server::Server::Server(std::uint32_t port)
    : _port(port), _io_context(), _threadAmount(std::thread::hardware_concurrency()), _threadPool(_threadAmount),
      _udp(_io_context, _port) {
}

rtype::server::Server::~Server() {
}

void rtype::server::Server::run() {
    for (unsigned int i = 0; i < _threadAmount; i++) {
        boost::asio::post(_threadPool, [&]() { _udp.run(); });
    }
    _threadPool.join();
}
