#include "protocols/UDPProtocol.hpp"
#include "protocols/AProtocol.hpp"
#include <boost/asio/placeholders.hpp>
#include <iostream>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>
#include <sstream>
#include <string>

rtype::server::UDPProtocol::UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port,
    SafeMessageQueue &SafeMessageQueue, ClientMessageQueueHandler &clientMQHandler)
    : AProtocol(io_context, port, SafeMessageQueue, clientMQHandler),
      _socket(_io_context, udp::endpoint(udp::v4(), _port)), _endpoint(), _buffer() {
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

        try {
            packets::PacketHeader header;
            std::stringstream ss(str);
            packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);

            packets::Serialization::deserialize(ar, header);

            _messageQueue.push(str);

            try {
                while (!_clientMQHandler.isClientEmpty(header.playerUuid)) {
                    std::string poppedMessage = _clientMQHandler.popMessage(header.playerUuid);
                    std::shared_ptr<std::string> message = std::make_shared<std::string>(poppedMessage);

                    _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
                        std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }
            } catch (std::exception &e) { std::cerr << e.what() << std::endl; }
        } catch (std::exception &) {
            std::cerr << "Couldn't decrypt header, message sent is probably not a packet" << std::endl;
        }

        startReceive();
    }
}

void rtype::server::UDPProtocol::handleSend(std::shared_ptr<std::string>, const boost::system::error_code &,
    std::size_t) {
}

void rtype::server::UDPProtocol::run() {
    _io_context.run();
}
