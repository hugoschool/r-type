#include "f-network/server/AProtocol.hpp"

fnetwork::server::AProtocol::AProtocol(boost::asio::io_context &io_context, std::uint16_t port)
    : _io_context(io_context), _port(port), _buffer() {
}
