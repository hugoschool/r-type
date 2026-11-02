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

        packets::Packet packet;

        auto *header = new packets::PacketHeader();
        header->set_type(packets::TypeLoginRequest);
        header->set_tick(_tick);
        header->set_playeruuid("");

        packet.set_allocated_header(header);

        packets::Serialization::serialize(ss, packet);

        std::string send = ss.str();
        _udp.send(send);
    }

    {
        boost::system::error_code error;
        std::string loginAnswerStr = _udp.read(error);
        std::stringstream loginAnswerSs(loginAnswerStr);

        packets::Packet packet;

        packets::Serialization::deserialize(loginAnswerSs, packet);
        _uuid = std::string(packet.loginanswer().uuid());

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

            packets::Packet packet;

            auto *header = new packets::PacketHeader();
            header->set_type(packets::TypePlayerEvent);
            header->set_tick(_tick);
            header->set_playeruuid(_uuid);

            packet.set_allocated_header(header);

            rtype::packets::Event input = _inputs.getElem();
            auto *payload = new packets::PlayerEvent();
            payload->set_event(input);

            packet.set_allocated_playerevent(payload);

            // auto &ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ss, packet);

            std::string buf = ss.str();
            _udp.send(buf);
        } else {
            // Send Ack if nothing else
            std::stringstream ss;

            packets::Packet packet;

            auto *header = new packets::PacketHeader();
            header->set_type(packets::PacketType::TypeAck);
            header->set_tick(_tick);
            header->set_playeruuid(_uuid);

            packet.set_allocated_header(header);

            packets::Serialization::serialize(ss, packet);

            std::string buf = ss.str();
            _udp.send(buf);
        }
    } catch (std::exception &) {}
}

void rtype::client::ClientProtocol::setSocketNonBlocking() {
    _udp.setSocketNonBlocking();
}
