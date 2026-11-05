#pragma once

#include "r-type/packets/Packet.hpp"
#include <cstddef>
#include <map>
#include <mutex>
#include <r-type/packets/Payload.hpp>
#include <string>
#include <utility>
#include <variant>

namespace rtype::server {
    using payloadType = std::variant<packets::EntitySpawn, packets::EntityDied>;
    class PriorityHandler {
        public:
            PriorityHandler();
            ~PriorityHandler();

            void addClient(std::string uuid);
            void addMessage(std::string uuid, size_t id, rtype::packets::PacketType, payloadType message);
            std::map<size_t, std::pair<packets::PacketType, payloadType>> getMessage(std::string uuid);
            void addAll(size_t id, rtype::packets::PacketType, payloadType message);
            void removeClient(std::string uuid);
            void removeMessage(std::string uuid, size_t id);

        private:
            std::map<std::string, std::map<size_t, std::pair<packets::PacketType, payloadType>>>
                _playersHighPriorityMsg;
            std::mutex _mutex;
    };
}
