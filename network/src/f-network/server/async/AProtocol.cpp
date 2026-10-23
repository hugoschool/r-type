#include "f-network/server/async/AProtocol.hpp"

fnetwork::server::async::AProtocol::AProtocol(boost::asio::io_context &io_context, std::uint16_t port)
    : _io_context(io_context), _port(port), _buffer() {
}
