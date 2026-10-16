#include "Client.hpp"
#include "UDPProtocol.hpp"
#include "boost/asio/error.hpp"
#include "boost/system/detail/error_code.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/SFMLDisplayModule.hpp"
#include "f-engine/modules/display/events/EventType.hpp"
#include "f-engine/modules/display/events/IEvent.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

rtype::client::Client::Client()
    : _displayModule(std::make_unique<fengine::modules::display::SFMLDisplayModule>()), _registery(),
      _writeThread(), _readThread(), _exit(false), _inputs(1024),
      _protocol(std::make_unique<UDPProtocol>(_inputs))
    {
}

rtype::client::Client::~Client() {
    _writeThread.join();
    _readThread.join();
    _protocol->close();
}

void rtype::client::Client::run(char *av[]) {
    _protocol->connect(av);
    _writeThread = std::thread(&rtype::client::Client::sendLoop, this);
    _readThread = std::thread(&rtype::client::Client::readLoop, this);
    loop();
}

void rtype::client::Client::sendLoop() {
    while (!_exit) {
        _protocol->sendMsg();
    }
    std::cout << "BYE BYE" << std::endl;
}

void rtype::client::Client::readLoop() {
    try {
        _protocol->setSocketNonBlocking();
        boost::system::error_code error  = boost::asio::error::would_block;
        while (error == boost::asio::error::would_block && !_exit) {
            _protocol->readMsg(error);
        }
        if (!_exit)
            readLoop();
    } catch (std::exception &) {
    }
}

void rtype::client::Client::loop() {

    while (true) {
        _displayModule->clear();

        _registery.systemManager.update(_registery);

        std::optional<std::unique_ptr<fengine::modules::display::IEvent>> event = _displayModule->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == fengine::modules::display::EventType::Quit) {
                _exit = true;
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
    }
}
