#include "ClientProtocol.hpp"
#include "CircularBuffer.hpp"
#include "f-network/client/UDPProtocol.hpp"
#include "r-type/event/PlayerEvent.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Payload.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstddef>
#include <iostream>

rtype::client::ClientProtocol::ClientProtocol(rtype::client::CircularBuffer<PlayerEvent> &inputs, std::string host,
    std::string port, std::size_t &tick)
    : _inputs(inputs), _io_context(), _udp(_io_context, host, std::stoi(port)), _packetBuilder(), _tick(tick) {
    _udp.connect();
    sendConnectionPackets();
}

rtype::client::ClientProtocol::~ClientProtocol() {
    _udp.close();
}

void rtype::client::ClientProtocol::sendConnectionPackets() {
    {
        std::string content = _packetBuilder.create(packets::PacketType::LoginRequest).addHeader(packets::PacketPriority::Low, 0).get();

        _udp.send(content);
    }

    {
        boost::system::error_code error;
        std::string loginAnswerStr = _udp.read(error);
        std::stringstream loginAnswerSs(loginAnswerStr);
        auto ar = packets::Serialization::createDeserializeArchive(loginAnswerSs);

        packets::PacketHeader header;
        packets::LoginAnswer payload;

        packets::Serialization::deserialize(ar, header);
        packets::Serialization::deserialize(ar, payload);

        std::string uuid = payload.uuid;
        _packetBuilder.setUuid(uuid);

        std::cout << "UUID: " << uuid << std::endl;
    }
}

std::string rtype::client::ClientProtocol::read(boost::system::error_code &error) {
    return _udp.read(error);
}

void rtype::client::ClientProtocol::sendMessage() {
    try {
        if (_inputs.canRead()) {
            rtype::event::Event input = _inputs.getElem();
            packets::PlayerEvent payload = {input};

            std::string content =
                _packetBuilder.create(packets::PacketType::PlayerEvent).addHeader(packets::PacketPriority::Low, 0).addPayload(payload).get();

            _udp.send(content);
        } else {
            // Send Ack if nothing else
            std::string content = _packetBuilder.create(packets::PacketType::Ack).addHeader(packets::PacketPriority::Low, 0).get();

            _udp.send(content);
        }
    } catch (std::exception &) {}
}

void rtype::client::ClientProtocol::setSocketNonBlocking() {
    _udp.setSocketNonBlocking();
}
