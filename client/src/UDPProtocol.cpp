#include "UDPProtocol.hpp"
#include "AProtocol.hpp"
#include "CircularBuffer.hpp"
#include "boost/asio/socket_base.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstring>
#include <iostream>
#include <iterator>

using EventKey = fengine::modules::display::EventKey;

rtype::client::UDPProtocol::UDPProtocol(rtype::client::CircularBuffer<EventKey> &input)
    : rtype::client::AProtocol(input), _resolver(_io_context), _endpoint(), _socket(_io_context) {
}

rtype::client::UDPProtocol::~UDPProtocol() {
}

void rtype::client::UDPProtocol::connect(char *av[]) {
    const std::string host = av[1];
    const std::string port = av[2];

    _endpoint = *_resolver.resolve(udp::v4(), host, port).begin();

    _socket.open(udp::v4());

    {
        std::stringstream ss;

        packets::PacketHeader header = {
            .type = packets::PacketType::LoginRequest,
            .size = 0,
            .playerUuid = "",
        };

        auto ar = packets::Serialization::createSerializeArchive(ss);
        packets::Serialization::serialize(ar, header);

        std::string send = ss.str();
        _socket.send_to(boost::asio::buffer(send), _endpoint);
    }

    {
        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        _socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::string loginAnswerStr(std::begin(recv_buf), std::end(recv_buf));
        std::stringstream loginAnswerSs(loginAnswerStr);
        auto ar = packets::Serialization::createDeserializeArchive(loginAnswerSs);

        packets::PacketHeader header;

        packets::Serialization::deserialize(ar, header);
        _uuid = header.playerUuid;

        std::cout << "UUID: " << _uuid << std::endl;
    }
}

void rtype::client::UDPProtocol::sendMsg() {
    try {
        if (_inputs.canRead()) {
            std::stringstream ss;

            packets::PacketHeader header = {
                .type = packets::PacketType::InputPlayer,
                .size = sizeof(packets::Input),
                .playerUuid = "",
            };

            std::strncpy(header.playerUuid, _uuid.c_str(), packets::UUID_SIZE);

            fengine::modules::display::EventKey input = _inputs.getElem();
            packets::Input newInput = {input};

            auto ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ar, header);
            packets::Serialization::serialize(ar, newInput);

            std::string buf = ss.str();
            _socket.send_to(boost::asio::buffer(buf), _endpoint);
        } else {
            // Send Ack if nothing else
            std::stringstream ss;

            packets::PacketHeader header = {
                .type = packets::PacketType::Ack,
                .size = 0,
                .playerUuid = "",
            };

            std::strncpy(header.playerUuid, _uuid.c_str(), packets::UUID_SIZE);

            auto ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ar, header);

            std::string buf = ss.str();
            _socket.send_to(boost::asio::buffer(buf), _endpoint);
        }
    } catch (std::exception &) {}
}

std::string rtype::client::UDPProtocol::readMsg(boost::system::error_code &error) {
    _buffer.fill(0);

    _socket.receive_from(boost::asio::buffer(_buffer), _endpoint, boost::asio::socket_base::wait_read, error);
    std::string msg(std::begin(_buffer), std::end(_buffer));
    return msg;
}

void rtype::client::UDPProtocol::setSocketNonBlocking() {
    _socket.non_blocking(true);
}

void rtype::client::UDPProtocol::close() {
    _socket.close();
}
