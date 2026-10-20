#include "protocols/UDPProtocol.hpp"
#include "ClientHandler.hpp"
#include "protocols/AProtocol.hpp"
#include <boost/asio/placeholders.hpp>
#include <iostream>
#include <r-type/packets/Entity.hpp>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>
#include <sstream>
#include <string>

rtype::server::UDPProtocol::UDPProtocol(boost::asio::io_context &io_context, std::uint16_t port,
    SafeMessageQueue &SafeMessageQueue, ClientHandler &clientHandler)
    : AProtocol(io_context, port, SafeMessageQueue, clientHandler),
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

            {
                std::stringstream ss(str);
                packets::Serialization::deserialize_archive ar = packets::Serialization::createDeserializeArchive(ss);

                packets::Serialization::deserialize(ar, header);
            }

            bool addMessageToQueue = true;
            std::string playerUuid(header.playerUuid);

            // Temporary queue for important messages, ex: LoginFailed
            std::queue<std::string> instantQueue;

            // A login request is a very important message, it must be handled on the spot, hence why it is here.
            // We cannot wait for another tick, since the client must know its UUID for the ACK requests.
            if (header.type == packets::PacketType::LoginRequest && !_clientHandler.clientExists(playerUuid)) {
                // TODO: handle a reconnecting player
                if (_clientHandler.getClientsAmount() + 1 > MAX_PLAYERS) {
                    packets::PacketHeader header = {
                        .type = packets::PacketType::LoginFailed,
                        .size = 0,
                        .playerUuid = "",
                    };

                    std::stringstream ss;
                    auto ar = packets::Serialization::createSerializeArchive(ss);
                    packets::Serialization::serialize(ar, header);
                    instantQueue.push(ss.str());

                    addMessageToQueue = false;
                } else {
                    playerUuid = _clientHandler.newClient();
                    _clientHandler.setAlive(playerUuid);
                }
            }

            if (addMessageToQueue) {
                _messageQueue.push(str);
            }

            try {
                while (!instantQueue.empty()) {
                    std::string str = instantQueue.front();
                    instantQueue.pop();
                    std::shared_ptr<std::string> message = std::make_shared<std::string>(str);

                    _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
                        std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }

                ClientMessageQueueHandler &clientMessageQueue = _clientHandler.getMessageQueueHandler();

                // Always respond even if the queue is empty as to not block the client
                if (clientMessageQueue.isClientEmpty(playerUuid)) {
                    packets::PacketHeader header = {
                        .type = packets::PacketType::Ack,
                        .size = 0,
                        .playerUuid = "",
                    };

                    std::stringstream ss;
                    auto ar = packets::Serialization::createSerializeArchive(ss);
                    packets::Serialization::serialize(ar, header);

                    std::shared_ptr<std::string> message = std::make_shared<std::string>(ss.str());

                    _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
                        std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }

                while (!clientMessageQueue.isClientEmpty(playerUuid)) {
                    std::string poppedMessage = clientMessageQueue.popMessage(playerUuid);
                    std::shared_ptr<std::string> message = std::make_shared<std::string>(poppedMessage);
                    _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
                        std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
                }
            } catch (std::exception &e) {
                std::cerr << "Message queue error: " << e.what() << std::endl;

                // Escalate error as to still send a packet incase of a random unreliable client error
                throw e;
            }
        } catch (std::exception &) {
            // TODO: determine whether we should send a packet here
            // Maybe it can happen that the game sends us a weird unreliable packet
            //
            // This may block the client randomly, the question is: should we send a packet
            // as to "unblock" the client or shall we continue normally?
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
