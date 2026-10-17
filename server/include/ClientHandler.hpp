#pragma once

#include "SafeQueue.hpp"
#include <chrono>
#include <map>
#include <string>
#include <unordered_map>

namespace rtype::server {
    // Maximum number of seconds before a client is considered as disconnected
    constexpr std::uint64_t MAX_CONNECTED_SECONDS = 10;
    // Maximum number of seconds before a client is no longer considered connecting
    constexpr std::uint64_t MAX_CONNECTING_SECONDS = 60;
    // Maximum number of seconds before a client is removed
    constexpr std::uint64_t MAX_DISCONNECTED_SECONDS = 60;

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
            std::chrono::steady_clock::time_point timer;
    };

    class ClientMessageQueueHandler {
        public:
            explicit ClientMessageQueueHandler();
            ~ClientMessageQueueHandler() = default;

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
            explicit ClientHandler();
            ~ClientHandler() = default;

            static std::string generateUuid();

            void addClient(std::string &uuid);
            void setStatus(std::string &uuid, ClientStatus newStatus);
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
            ClientMessageQueueHandler _messageQueueHandler;
    };
}
