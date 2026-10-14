#pragma once

#include "ClientHelper.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include "f-engine/display_module/IDisplayModule.hpp"
#include <memory>
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

            void connect(char *av[]);
            void loop();
        public:
            Client();
            ~Client();

            ClientHelper helper;
            void run(char *av[]);
    };
}
