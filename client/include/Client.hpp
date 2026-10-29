#pragma once

#include "CircularBuffer.hpp"
#include "ClientProtocol.hpp"
#include "f-engine/camera/Camera.hpp"
#include "f-engine/config/IConfig.hpp"
#include "f-engine/ecs/Registry.hpp"
#include "f-engine/modules/display/IDisplayModule.hpp"
#include "f-engine/modules/sound/ISoundModule.hpp"
#include "f-engine/scene/IScene.hpp"
#include "r-type/event/PlayerEvent.hpp"
#include "r-type/packets/Serialization.hpp"
#include <array>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstddef>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

constexpr std::size_t INTERPOLATION_TICK = 5;

namespace rtype::client {
    class Client {
            using udp = boost::asio::ip::udp;

        private:
            std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
            std::unique_ptr<fengine::modules::sound::ISoundModule> _soundModule;
            fengine::ecs::Registry _registery;
            fengine::camera::Camera _camera;
            std::vector<std::shared_ptr<fengine::scene::IScene>> _scenes;
            std::shared_ptr<fengine::scene::IScene> _currentScene;
            std::thread _writeThread;
            std::thread _readThread;
            bool _exit;
            rtype::client::CircularBuffer<rtype::event::Event> _inputs;
            std::string _uuid;
            ClientProtocol _protocol;
            std::shared_ptr<fengine::config::IConfig> _clientConfig;
            std::array<std::pair<std::size_t, std::queue<std::string>>, 10> _packetReceived;
            std::size_t _maxTick;
            bool _startedPlaying;

            void loop();
            void sendLoop();
            void readLoop();

            void interpretMsg(std::string msg);
            void stockMsg(std::string msg);
            void removeEntity(packets::Serialization::deserialize_archive &ar);
            void addEntity(packets::Serialization::deserialize_archive &ar);
            void moveEntity(packets::Serialization::deserialize_archive &ar);
            void modifyVelocity(packets::Serialization::deserialize_archive &ar);

            void interpretButtonAction(int);

            void applyReceivedPackets(void);

        public:
            Client(std::string host, std::string port);
            ~Client();

            void run();
    };
}
