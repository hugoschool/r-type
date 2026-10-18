#pragma once

#include "CircularBuffer.hpp"
#include "IProtocol.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <memory>
#include <thread>

namespace rtype::client {
    class Client {
            using udp = boost::asio::ip::udp;

        private:
            std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
            fengine::ecs::Registry _registery;
            std::thread _writeThread;
            std::thread _readThread;
            bool _exit;
            rtype::client::CircularBuffer<fengine::modules::display::EventKey> _inputs;
            std::string _uuid;
            std::unique_ptr<IProtocol> _protocol;

            void loop();
            void sendLoop();
            void readLoop();
            void interpretMsg(std::string msg);

        public:
            Client();
            ~Client();

            void run(char *av[]);
    };
}
