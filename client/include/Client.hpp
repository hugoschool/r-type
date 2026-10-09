#pragma once

#include "ClientHelper.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include "f-engine/display_module/IDisplayModule.hpp"
#include <memory>
#include "f-engine/ecs/Registry.hpp"

namespace rtype {
    namespace client {
        class Client {
            private:
                boost::asio::io_context _io_context;
                udp::resolver _resolver;
                udp::endpoint _endpoint;
                udp::socket _socket;
                std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
                fengine::ecs::Registry _registery;

                int connect(char *av[]);
                int loop();

            public:
                using udp = boost::asio::ip::udp;

                Client();
                ~Client();

                ClientHelper helper;
        };
    }
}
