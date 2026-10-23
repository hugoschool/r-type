#pragma once

#include "CircularBuffer.hpp"
#include "ClientProtocol.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/config/IConfig.hpp"
#include "f-engine/ecs/Entity.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"
#include "f-engine/scene/IScene.hpp"
#include "r-type/packets/Serialization.hpp"
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
            std::vector<std::shared_ptr<fengine::scene::IScene>> _scenes;
            fengine::ecs::Entity _entity;
            std::thread _writeThread;
            std::thread _readThread;
            bool _exit;
            rtype::client::CircularBuffer<fengine::modules::display::EventKey> _inputs;
            std::string _uuid;
            std::unique_ptr<IProtocol> _protocol;
            std::shared_ptr<fengine::config::IConfig> _clientConfig;

            void loop();
            void sendLoop();
            void readLoop();

            void interpretMsg(std::string msg);
            void removeEntity(packets::Serialization::deserialize_archive &ar);
            void addEntity(packets::Serialization::deserialize_archive &ar);
            void moveEntity(packets::Serialization::deserialize_archive &ar);
            void modifyVelocity(packets::Serialization::deserialize_archive &ar);

            void interpretButtonAction(int);

        public:
            Client(std::string host, std::string port);
            ~Client();

            void run();
    };
}
