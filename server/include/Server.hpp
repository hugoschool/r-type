#pragma once

#include "ClientHandler.hpp"
#include "ServerProtocol.hpp"
#include "SubscriberHandler.hpp"
#include "TickHandler.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <cstddef>
#include <cstdint>
#include <f-engine/ecs/Registry.hpp>
#include <f-engine/modules/display/events/EventKey.hpp>
#include <f-engine/world/World.hpp>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/PacketBuilder.hpp>
#include <r-type/packets/Serialization.hpp>
#include <r-type/presets/EntitiesPresets.hpp>

namespace rtype::server {
    class Server {
        public:
            explicit Server(std::uint32_t port);
            ~Server();

            void run();
            void tickThreadRun();
            void interpretPacket(packets::Serialization::deserialize_archive &ar, packets::PacketHeader &header);

        private:
            std::uint32_t _port;

            boost::asio::io_context _io_context;

            unsigned int _totalThreadAmount;
            unsigned int _protocolThreadAmount;
            boost::asio::thread_pool _threadPool;

            bool _stop;
            boost::asio::signal_set _signals;

            fengine::world::World _world;

            SafeQueue<std::size_t> _newClientQueue;

            TickHandler _tickHandler;
            ClientHandler _clientHandler;
            SubscriberHandler _subscriberHandler;

            ServerProtocol _protocol;
            ServerPacketBuilder _packetBuilder;

            std::chrono::steady_clock::time_point _spawnClock;
            short _enemyCounter;

            void generateEnemies();

            std::queue<std::size_t> generateClientQueue();

            void newEntityPacket(std::size_t entityId, fengine::presets::EntityType entityType, std::size_t tick,
                std::string uuid);
            void userLoginPacket(std::size_t tick, std::string uuid);
            void handlePlayerEvents(packets::Serialization::deserialize_archive &ar, packets::PacketHeader &header);
            void handleVelocityChanges(float &velocity, float counterVelocity, float value);

            std::chrono::steady_clock::time_point _resendClock;
            void checkResend();

            bool _gameStarted;
            std::size_t _level;
    };
}
