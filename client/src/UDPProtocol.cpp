#include "UDPProtocol.hpp"
#include "AProtocol.hpp"
#include "CircularBuffer.hpp"
#include "boost/asio/socket_base.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <iterator>

using EventKey = fengine::modules::display::EventKey;

rtype::client::UDPProtocol::UDPProtocol(rtype::client::CircularBuffer<EventKey> &input) :
    rtype::client::AProtocol(input), _resolver(_io_context), _endpoint(), _socket(_io_context) {
}

rtype::client::UDPProtocol::~UDPProtocol() {
}

void rtype::client::UDPProtocol::connect(char *av[]) {
    const std::string host = av[1];
    const std::string port = av[2];

    _endpoint = *_resolver.resolve(udp::v4(), host, port).begin();

    _socket.open(udp::v4());
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

    // Connect to the server and send connection information to it.

    // the cient spams send the henshake to the server
    // when the client recieve the server response it stops
    // the server now sends all his ecs informations
    // the client now feed all these information into his ecs to match the server's one
    // throw an exception if the client dont achieve to connect to the server
}

void rtype::client::UDPProtocol::sendMsg() {
    std::stringstream ss;

    packets::PacketHeader header = {
        .type = packets::PacketType::InputPlayer,
        .size = sizeof(packets::Input),
        .playerUuid = "f52ea348-6c57-4acf-9834-059bc9a9c81c",
    };

    try {
        if (_inputs.canRead()) {
            fengine::modules::display::EventKey input = _inputs.getElem();
            packets::Input newInput = {input};
            packets::Serialization::serialize(ss, header, newInput);
            std::string buf = ss.str();
            _socket.send_to(boost::asio::buffer(buf), _endpoint);
        }
    } catch (std::exception &) {
    }
}

std::string rtype::client::UDPProtocol::readMsg(boost::system::error_code &error) {
    _buffer.fill(0);

    _socket.receive_from(boost::asio::buffer(_buffer), _endpoint, boost::asio::socket_base::wait_read, error);
    std::string test(std::begin(_buffer), std::end(_buffer));
    return test;
}

void rtype::client::UDPProtocol::setSocketNonBlocking()
{
    _socket.non_blocking(true);
}

void rtype::client::UDPProtocol::close()
{
    _socket.close();
}
