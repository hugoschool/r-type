#include "ClientProtocol.hpp"
#include "CircularBuffer.hpp"
#include "f-network/client/UDPProtocol.hpp"
#include "r-type/packets/Serialization.hpp"
#include "r-type/packets/Variables.hpp"
#include <cstddef>
#include <iostream>
#include <packets.pb.h>

rtype::client::ClientProtocol::ClientProtocol(rtype::client::CircularBuffer<PlayerEvent> &inputs, std::string host,
    std::string port, std::size_t &tick)
    : _inputs(inputs), _io_context(), _udp(_io_context, host, std::stoi(port)), _packetBuilder(packets::DUMMY_UUID),
      _tick(tick) {
    _udp.connect();
    sendConnectionPackets();
}

rtype::client::ClientProtocol::~ClientProtocol() {
    _udp.close();
}

void rtype::client::ClientProtocol::sendConnectionPackets() {
    {
        std::string content = _packetBuilder.create(0, packets::TypeLoginRequest).addHeader().get();
        _udp.send(content);
    }

    {
        boost::system::error_code error;
        std::string loginAnswerStr = _udp.read(error);
        std::stringstream loginAnswerSs(loginAnswerStr);

        packets::Packet packet;

        packets::Serialization::deserialize(loginAnswerSs, packet);

        std::string uuid = packet.loginanswer().uuid();
        std::cout << "UUID: " << uuid << std::endl;
        _packetBuilder.setUuid(uuid);
    }
}

std::string rtype::client::ClientProtocol::read(boost::system::error_code &error) {
    return _udp.read(error);
}

void rtype::client::ClientProtocol::sendMessage() {
    try {
        if (_inputs.canRead()) {
            rtype::packets::Event input = _inputs.getElem();
            auto *payload = new packets::PlayerEvent();
            payload->set_event(input);

            std::string content =
                _packetBuilder.create(0, packets::TypePlayerEvent).addHeader().addPayload(payload).get();
            _udp.send(content);
        } else {
            // Send Ack if nothing else
            std::string content = _packetBuilder.create(0, packets::TypeAck).addHeader().get();
            _udp.send(content);
        }
    } catch (std::exception &) {}
}

void rtype::client::ClientProtocol::setSocketNonBlocking() {
    _udp.setSocketNonBlocking();
}
