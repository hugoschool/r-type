#include "f-network/server/async/UDPProtocol.hpp"
#include <boost/asio/placeholders.hpp>
#include <string>

fnetwork::server::async::UDPProtocol::UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port)
    : AProtocol(io_context, port), _socket(_io_context, udp::endpoint(udp::v4(), _port)), _endpoint() {
    startReceive();
}

void fnetwork::server::async::UDPProtocol::startReceive() {
    _buffer.fill(0);

    _socket.async_receive_from(boost::asio::buffer(_buffer), _endpoint,
        std::bind(&UDPProtocol::handleReceive, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void fnetwork::server::async::UDPProtocol::handleSend(std::shared_ptr<std::string>, const boost::system::error_code &,
    std::size_t) {
}

void fnetwork::server::async::UDPProtocol::run() {
    _io_context.run();
}
