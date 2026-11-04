#pragma once

#include "ClientHandler.hpp"
#include "PriorityHandler.hpp"
#include "ServerProtocol.hpp"
#include "SubscriberHandler.hpp"
#include "TickHandler.hpp"
#include "f-engine/ecs/components/PositionComponent.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/PlayerBonusComponent.hpp"
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

            ServerProtocol _protocol;
            ServerPacketBuilder _packetBuilder;

            void generateEnemies();

            std::queue<std::size_t> generateClientQueue();

            void newEntityPacket(std::size_t entityId, fengine::presets::EntityType entityType, std::size_t tick,
                std::string uuid);
            void userLoginPacket(std::size_t tick, std::string uuid);
            void handlePlayerEvents(packets::Serialization::deserialize_archive &ar, packets::PacketHeader &header);
            void handleVelocityChanges(float &velocity, float counterVelocity, float value);
            void handlePlayerShots(fengine::ecs::PositionComponent &pos, fengine::ecs::AttackComponent &att,
                fengine::ecs::PlayerBonusComponent &bonus, float velX, float velY);
            void resetPlayerBonus(fengine::ecs::PlayerBonusComponent &);

            std::chrono::steady_clock::time_point _resendClock;
            void checkResend();

            std::chrono::steady_clock::time_point _spawnClock;
            std::size_t _enemyCounter;

            bool _gameStarted;
            std::size_t _level;
            std::size_t _enemyAlive;

            void addPriorityMessages();
            rtype::server::PriorityHandler _priorityHandler;
            ClientHandler _clientHandler;
            SubscriberHandler _subscriberHandler;
    };
}
