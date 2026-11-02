#include "ClientProtocol.hpp"
#include "CircularBuffer.hpp"
#include "f-network/client/UDPProtocol.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstddef>
#include <iostream>
#include <r-type/proto/packets.pb.h>

rtype::client::ClientProtocol::ClientProtocol(rtype::client::CircularBuffer<PlayerEvent> &inputs, std::string host,
    std::string port, std::size_t &tick)
    : _inputs(inputs), _io_context(), _udp(_io_context, host, std::stoi(port)), _uuid(), _tick(tick) {
    _udp.connect();
    sendConnectionPackets();
}

rtype::client::ClientProtocol::~ClientProtocol() {
    _udp.close();
}

void rtype::client::ClientProtocol::sendConnectionPackets() {
    {
        std::stringstream ss;

        packets::PacketHeader header;
        header.set_type(packets::TypeLoginRequest);
        header.set_tick(_tick);
        header.set_playeruuid("");

        packets::Serialization::serialize(ss, header);

        std::string send = ss.str();
        _udp.send(send);
    }

    {
        boost::system::error_code error;
        std::string loginAnswerStr = _udp.read(error);
        std::stringstream loginAnswerSs(loginAnswerStr);

        packets::PacketHeader header;
        packets::LoginAnswer payload;

        packets::Serialization::deserialize(loginAnswerSs, header);
        packets::Serialization::deserialize(loginAnswerSs, payload);
        _uuid = std::string(payload.uuid());

        std::cout << "UUID: " << _uuid << std::endl;
    }
}

std::string rtype::client::ClientProtocol::read(boost::system::error_code &error) {
    return _udp.read(error);
}

void rtype::client::ClientProtocol::sendMessage() {
    try {
        if (_inputs.canRead()) {
            std::stringstream ss;

            packets::PacketHeader header;
            header.set_type(packets::TypePlayerEvent);
            header.set_tick(_tick);
            header.set_playeruuid(_uuid);

            rtype::packets::Event input = _inputs.getElem();
            packets::PlayerEvent newInput;
            newInput.set_event(input);

            // auto &ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ss, header);
            packets::Serialization::serialize(ss, newInput);

            std::string buf = ss.str();
            _udp.send(buf);
        } else {
            // Send Ack if nothing else
            std::stringstream ss;

            packets::PacketHeader header;
            header.set_type(packets::PacketType::TypeAck);
            header.set_tick(_tick);
            header.set_playeruuid(_uuid);

            packets::Serialization::serialize(ss, header);

            std::string buf = ss.str();
            _udp.send(buf);
        }
    } catch (std::exception &) {}
}

void rtype::client::ClientProtocol::setSocketNonBlocking() {
    _udp.setSocketNonBlocking();
}
