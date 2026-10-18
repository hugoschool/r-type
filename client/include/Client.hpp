#pragma once

#include "CircularBuffer.hpp"
#include "IProtocol.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include "r-type/packets/Entity.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstddef>
#include <memory>
#include <thread>
#include <unordered_map>

namespace rtype::client {


    // TODO: change the entities
    struct Entity {
        public:
            size_t _id;
            packets::Vector2D _pos;
            packets::Vector2D _vel;
    };

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

            // TODO: change this
            std::unordered_map<size_t, Entity> _entities;

            void loop();
            void sendLoop();
            void readLoop();

            void interpretMsg(std::string msg);
            void removeEntity(packets::Serialization::deserialize_archive &ar);
            void addEntity(packets::Serialization::deserialize_archive &ar);


        public:
            Client();
            ~Client();

            void run(char *av[]);
    };
}
