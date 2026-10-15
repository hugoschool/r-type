#pragma once

#include "ClientHelper.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include "f-engine/modules/display/IDisplayModule.hpp"
#include <memory>
#include <thread>
#include "CircularBuffer.hpp"
#include "f-engine/ecs/Registry.hpp"

namespace rtype::client {
    class Client {
            using udp = boost::asio::ip::udp;

        private:
            boost::asio::io_context _io_context;
            udp::resolver _resolver;
            udp::endpoint _endpoint;
            udp::socket _socket;
            std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
            fengine::ecs::Registry _registery;
            std::thread _writeThread;
            std::thread _readThread;
            bool _exit;
            rtype::client::CircularBuffer<fengine::modules::display::EventKey> _inputs;
            std::string _uuid;

            void connect(char *av[]);
            void loop();
            void communicationLoop(char *av[]);
            void sendMsg();
            void readMsg();

        public:
            Client();
            ~Client();

            ClientHelper helper;
            void run(char *av[]);
    };
}
