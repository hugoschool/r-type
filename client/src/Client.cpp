#include "Client.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
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

    packets::PacketHeader header = {
        .type = packets::PacketType::Ack,
        .size = 0,
        // TODO: this is a simple test UUID for the server <-> client communication
        // To be removed later
        .playerUuid = "f52ea348-6c57-4acf-9834-059bc9a9c81c",
    };

    packets::Serialization::serialize(ss, header, packets::Empty{});

    std::string send = ss.str();
    _socket.send_to(boost::asio::buffer(send), _endpoint);

    while (true) {
        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = _socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }
}

rtype::client::Client::~Client() {
}
