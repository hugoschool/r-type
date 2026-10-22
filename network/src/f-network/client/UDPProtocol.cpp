#include "f-network/client/UDPProtocol.hpp"
#include "f-network/client/AProtocol.hpp"
#include <boost/asio/socket_base.hpp>
#include <iterator>

fnetwork::client::UDPProtocol::UDPProtocol(boost::asio::io_context &_io_context, std::string host, std::uint16_t port)
    : fnetwork::client::AProtocol(_io_context, host, port), _resolver(_io_context), _endpoint(), _socket(_io_context) {
}

void fnetwork::client::UDPProtocol::connect() {
    _endpoint = *_resolver.resolve(udp::v4(), _host, std::to_string(_port)).begin();
    _socket.open(udp::v4());
}

void fnetwork::client::UDPProtocol::close() {
    _socket.close();
}

void fnetwork::client::UDPProtocol::send(std::string content) {
    _socket.send_to(boost::asio::buffer(content), _endpoint);
}

std::string fnetwork::client::UDPProtocol::read(boost::system::error_code &error) {
    _buffer.fill(0);

    _socket.receive_from(boost::asio::buffer(_buffer), _endpoint, boost::asio::socket_base::wait_read, error);
    std::string msg(std::begin(_buffer), std::end(_buffer));
    return msg;
}

void fnetwork::client::UDPProtocol::setSocketNonBlocking() {
    _socket.non_blocking(true);
}
