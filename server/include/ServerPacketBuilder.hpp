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

            packets::PacketBuilderContent create(packets::PacketType type, packets::PacketPriority, size_t);
            size_t getCurrentId();

        private:
            TickHandler &_tick;
            std::mutex _mutex;
            size_t _id;
            void incrementID();
    };
}
