#include "protocols/UDPProtocol.hpp"
#include "protocols/AProtocol.hpp"
#include <boost/asio/placeholders.hpp>
#include <string>
#include <unistd.h>

rtype::server::UDPProtocol::UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port)
    : AProtocol(io_context, port), _socket(_io_context, udp::endpoint(udp::v4(), _port)), _endpoint(), _buffer() {
    startReceive();
}

void rtype::server::UDPProtocol::startReceive() {
    _buffer.fill(0);

    _socket.async_receive_from(boost::asio::buffer(_buffer), _endpoint,
        std::bind(&UDPProtocol::handleReceive, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void rtype::server::UDPProtocol::handleReceive(const boost::system::error_code &error, std::size_t) {
    if (!error) {
        std::string str(std::begin(_buffer), std::end(_buffer));
        std::shared_ptr<std::string> message = std::make_shared<std::string>(str);

        _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
            std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

        startReceive();
    }
}

void rtype::server::UDPProtocol::handleSend(std::shared_ptr<std::string>, const boost::system::error_code &,
    std::size_t) {
}

void rtype::server::UDPProtocol::run() {
    _io_context.run();
}
