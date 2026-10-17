#include "Client.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstring>
#include <iostream>

rtype::client::Client::Client() : _io_context(), _resolver(_io_context), _endpoint(), _socket(_io_context) {
    const std::string host = "127.0.0.1";
    const std::string port = "25000";
    _endpoint = *_resolver.resolve(udp::v4(), host, port).begin();

    _socket.open(udp::v4());
    connect();
}

void rtype::client::Client::connect() {
    std::stringstream ss;

    // Send login request
    {
        packets::PacketHeader header = {
            .type = packets::PacketType::LoginRequest,
            .size = 0,
            .playerUuid = "",
        };

        packets::Serialization::serialize(ss, header, packets::Empty{});

        std::string send = ss.str();
        _socket.send_to(boost::asio::buffer(send), _endpoint);
    }

    // Get the UUID from the LoginRequest
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
    }
}

void rtype::client::Client::run() {
    while (true) {
        // Send an ACK packet
        {
            std::stringstream ss;

            packets::PacketHeader header = {
                .type = packets::PacketType::Ack,
                .size = 0,
                .playerUuid = "",
            };

            std::strncpy(header.playerUuid, _uuid.c_str(), packets::UUID_SIZE);

            packets::Serialization::serialize(ss, header, packets::Empty{});

            std::string send = ss.str();
            _socket.send_to(boost::asio::buffer(send), _endpoint);
        }

        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = _socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout << "Data: ";
        std::cout.write(recv_buf.data(), len);
        std::cout << std::endl;
    }
}
