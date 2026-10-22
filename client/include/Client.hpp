#pragma once

#include "CircularBuffer.hpp"
#include "IProtocol.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "r-type/packets/Serialization.hpp"
#include "r-type/scene/GameScene.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <memory>
#include <queue>
#include <thread>

namespace rtype::client {

    class Client {
            using udp = boost::asio::ip::udp;

        private:
            std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
            fengine::ecs::Registry _registery;
            fengine::camera::Camera _camera;
            std::queue<fengine::modules::display::EventKey> _pressqueue;
            std::queue<fengine::modules::display::EventKey> _releasequeue;
            fengine::scene::GameScene _scene;
            fengine::ecs::Entity _entity;
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
            void removeEntity(packets::Serialization::deserialize_archive &ar);
            void addEntity(packets::Serialization::deserialize_archive &ar);
            void moveEntity(packets::Serialization::deserialize_archive &ar);
            void modifyVelocity(packets::Serialization::deserialize_archive &ar);

        public:
            Client();
            ~Client();

            void run(char *av[]);
    };
}
