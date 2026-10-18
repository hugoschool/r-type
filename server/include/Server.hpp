#pragma once

#include "ClientHandler.hpp"
#include "SubscriberHandler.hpp"
#include "TickHandler.hpp"
#include "protocols/UDPProtocol.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <cstdint>
#include <f-engine/ecs/Registry.hpp>
#include <f-engine/world/World.hpp>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>

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

            SubscriberHandler _subscriberHandler;
            fengine::world::World _world;

            TickHandler _tickHandler;
            ClientHandler _clientHandler;

            UDPProtocol _udp;

            std::chrono::steady_clock::time_point _spawnClock;
            short _enemyCounter;

            void generateEnemies();
            void userLoginPacket(std::string uuid);
    };
}
