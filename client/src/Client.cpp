#include "Client.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <thread>
#include <unistd.h>

rtype::client::Client::Client()
    : _io_context(), _resolver(_io_context), _endpoint(), _socket(_io_context),
      _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery(),
      _writeThread(), _readThread(), _exit(false), _inputs(1024)
    {
}

rtype::client::Client::~Client() {
    _socket.close();
    _writeThread.join();
    _readThread.join();
}

void rtype::client::Client::run(char *av[]) {
    connect(av);
    loop();
}

void rtype::client::Client::communicationLoop(char *av[]) {
    connect(av);
    _readThread = std::thread(&rtype::client::Client::readMsg, this);
    while (!_exit) {
        sleep(1);
        sendMsg();
    }
    std::cout << "BYE BYE" << std::endl;
}

void rtype::client::Client::readMsg() {

    char reply[1024];
    try {
        while (!_exit) {
            _socket.receive_from(boost::asio::buffer(reply, 1024), _endpoint);
        }
    } catch (std::exception &) {
    }
    std::cout << "TEST" << std::endl;
}

void rtype::client::Client::sendMsg() {
    std::stringstream ss;

    packets::PacketHeader header = {
        .type = packets::PacketType::InputPlayer,
        .size = 0,
        .playerUuid = "f52ea348-6c57-4acf-9834-059bc9a9c81c",
    };

    if (_inputs.canRead()) {
        fengine::modules::display::EventKey input = _inputs.getElem();
        packets::Input newInput = {_uuid, input};
        packets::Serialization::serialize(ss, header, newInput);
        std::string buf = ss.str();
        _socket.send_to(boost::asio::buffer(buf), _endpoint);
    }
}

void rtype::client::Client::connect(char *av[]) {
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

    while (true) {
        std::array<char, 1024> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = _socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }

    // Connect to the server and send connection information to it.

    // the cient spams send the henshake to the server
    // when the client recieve the server response it stops
    // the server now sends all his ecs informations
    // the client now feed all these information into his ecs to match the server's one
    // throw an exception if the client dont achieve to connect to the server
}

void rtype::client::Client::loop() {

    while (true) {
        _displayModule->clear();

        _registery.systemManager.update(_registery);

        std::optional<std::unique_ptr<fengine::modules::display::IEvent>> event = _displayModule->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == fengine::modules::display::EventType::Quit) {
                break;
            }
            if (event.value()->getType() == fengine::modules::display::EventType::KeyPressed) {
                _inputs.addElement(event.value()->getKey());
            }
        }

        // get player input
        // well format it with serialization
        // send it to the server
        // create prediction based on the input
        // get server response compare server response with the predition
    // Connect to the server and send connection information to it.

    // the cient spams send the henshake to the server
    // when the client recieve the server response it stops
    // the server now sends all his ecs informations
    // the client now feed all these information into his ecs to match the server's one
    // throw an exception if the client dont achieve to connect to the server

    }
}
