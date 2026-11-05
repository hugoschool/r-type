#pragma once

#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <cstddef>
#include <sstream>
#include <string>

namespace rtype::packets {
    class PacketBuilderContent {
        public:
            PacketBuilderContent(PacketType type, std::size_t tick, std::string uuid, rtype::packets::PacketPriority, size_t id);
            ~PacketBuilderContent() = default;

            PacketBuilderContent &addHeader();

            template <class T>
            PacketBuilderContent &addPayload(T payload) {
                Serialization::serialize(_ar, payload);
                return *this;
            };

            std::string get();

        private:
            std::stringstream _ss;
            Serialization::serialize_archive _ar;

            PacketType _type;
            std::size_t _tick;
            std::string _uuid;
            rtype::packets::PacketPriority _priority;
            size_t _id;
    };

    class PacketBuilder {
        public:
            PacketBuilder(std::string uuid);
            ~PacketBuilder() = default;

            PacketBuilderContent create(std::size_t tick, PacketType type, PacketPriority, size_t);
            void setUuid(std::string uuid);

        private:
            std::string _uuid;
    };
}
