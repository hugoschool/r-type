#pragma once

#include "TickHandler.hpp"
#include "r-type/packets/Packet.hpp"
#include <cstddef>
#include <mutex>
#include <r-type/packets/PacketBuilder.hpp>

namespace rtype::server {
    class ServerPacketBuilder : public packets::PacketBuilder {
        public:
            ServerPacketBuilder(TickHandler &tick);
            ~ServerPacketBuilder() = default;

            // Automatically increases the current id with incrementId function
            packets::PacketBuilderContent create(packets::PacketType type, packets::PacketPriority);

            // Doesn't increment ID, must be done manually after
            packets::PacketBuilderContent create(packets::PacketType type, packets::PacketPriority, std::size_t);

            std::size_t getCurrentId();
            void incrementId();

        private:
            TickHandler &_tick;
            std::mutex _mutex;
            std::size_t _id;
    };
}
