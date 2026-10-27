#include "ClientProtocol.hpp"
#include "CircularBuffer.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-network/client/UDPProtocol.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Payload.hpp"
#include "r-type/packets/Serialization.hpp"
#include <iostream>

rtype::client::ClientProtocol::ClientProtocol(rtype::client::CircularBuffer<EventKey> &inputs, std::string host,
    std::string port)
    : _inputs(inputs), _io_context(), _udp(_io_context, host, std::stoi(port)), _entityId(std::nullopt), _uuid() {
    _udp.connect();

    sendConnectionPackets();
}

rtype::client::ClientProtocol::~ClientProtocol() {
    _udp.close();
}

void rtype::client::ClientProtocol::sendConnectionPackets() {
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
        _udp.send(send);
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
        _uuid = payload.uuid;
        _entityId = payload.entityId;

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

            packets::PacketHeader header = {
                .type = packets::PacketType::InputPlayer,
                .size = sizeof(packets::Input),
                .playerUuid = "",
            };
            header.setUuid(_uuid);

            fengine::modules::display::EventKey input = _inputs.getElem();
            packets::Input newInput = {input};

            auto ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ar, header);
            packets::Serialization::serialize(ar, newInput);

            std::string buf = ss.str();
            _udp.send(buf);
        } else {
            // Send Ack if nothing else
            std::stringstream ss;

            packets::PacketHeader header = {
                .type = packets::PacketType::Ack,
                .size = 0,
                .playerUuid = "",
            };
            header.setUuid(_uuid);

            auto ar = packets::Serialization::createSerializeArchive(ss);
            packets::Serialization::serialize(ar, header);

            std::string buf = ss.str();
            _udp.send(buf);
        }
    } catch (std::exception &) {}
}

void rtype::client::ClientProtocol::setSocketNonBlocking() {
    _udp.setSocketNonBlocking();
}

std::optional<std::size_t> &rtype::client::ClientProtocol::getEntityId() {
    return _entityId;
}
