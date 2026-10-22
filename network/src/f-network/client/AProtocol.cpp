#include "f-network/client/AProtocol.hpp"
#include <boost/asio/io_context.hpp>

fnetwork::client::AProtocol::AProtocol(boost::asio::io_context &_io_context, std::string host, std::uint16_t port)
    : _io_context(_io_context), _host(host), _port(port), _buffer() {
}
