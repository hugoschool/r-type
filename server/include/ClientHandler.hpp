#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

namespace rtype::server {
    enum class ClientStatus {
        Disconnected,
        Connecting,
        Connected,
    };

    struct ClientData {
        public:
            ClientStatus status;
            std::chrono::steady_clock::time_point timer;
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

        private:
            std::unordered_map<std::string, ClientData> _clients;
    };
}
