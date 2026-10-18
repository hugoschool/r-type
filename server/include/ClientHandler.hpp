#pragma once

#include "SafeQueue.hpp"
#include "f-engine/ecs/Registry.hpp"
#include <chrono>
#include <map>
#include <r-type/packets/Packet.hpp>
#include <string>
#include <unordered_map>

namespace rtype::server {
    constexpr char DUMMY_UUID[packets::UUID_SIZE + 1] = "00000000-0000-4000-8000-000000000000";

    // Maximum number of seconds before a client is considered as disconnected
    constexpr std::uint64_t MAX_CONNECTED_SECONDS = 10;
    // Maximum number of seconds before a client is no longer considered connecting
    constexpr std::uint64_t MAX_CONNECTING_SECONDS = 60;
    // Maximum number of seconds before a client is removed
    constexpr std::uint64_t MAX_DISCONNECTED_SECONDS = 60;

    // Maximum amount of players that can be connected to a single server
    constexpr std::size_t MAX_PLAYERS = 4;

    enum class ClientStatus {
        Disconnected,
        Connecting,
        Connected,
        // Client can no longer reconnect
        Removed,
    };

    struct ClientData {
        public:
            ClientStatus status;
            std::size_t entityId;
            std::chrono::steady_clock::time_point timer;
    };

    class ClientMessageQueueHandler {
        public:
            explicit ClientMessageQueueHandler();
            ~ClientMessageQueueHandler() = default;

            void createClient(std::string uuid);
            void addMessage(std::string uuid, std::string content);
            std::string popMessage(std::string uuid);
            bool isClientEmpty(std::string uuid);
            void sendToAllClients(std::string content);
            std::map<std::string, SafeMessageQueue> &getMap();

        private:
            std::map<std::string, SafeMessageQueue> _map;
            std::mutex _mutex;
    };

    class ClientHandler {
        public:
            explicit ClientHandler(fengine::ecs::Registry &registry);
            ~ClientHandler() = default;

            static std::string generateUuid();

            std::string newClient();
            void setStatus(std::string &uuid, ClientStatus newStatus);
            bool clientExists(std::string &uuid);
            std::size_t getClientsAmount();
            // Simply resets the timer
            void setAlive(std::string &uuid);

            // Verifies the status of all clients
            //
            // If a client hasn't sent a packet in too long, he will be switched to disconnected.
            // If he has been disconnected for too long, he will be removed.
            void verifyStatus();

            ClientMessageQueueHandler &getMessageQueueHandler();

        private:
            std::unordered_map<ClientStatus, std::chrono::seconds> _maxStatusSecondsMap;
            std::unordered_map<std::string, ClientData> _clients;

            // Array of pregenerated UUIDs
            std::queue<std::string> _uuids;

            fengine::ecs::Registry &_registry;

            ClientMessageQueueHandler _messageQueueHandler;
    };
}
