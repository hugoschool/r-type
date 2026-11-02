#pragma once

#include "r-type/packets/Serialization.hpp"
#include <r-type/proto/packets.pb.h>
#include <sstream>
#include <string>

namespace rtype::packets {
    class PacketBuilderContent {
        public:
            PacketBuilderContent(PacketType type, std::size_t tick, std::string uuid);
            ~PacketBuilderContent() = default;

            PacketBuilderContent &addHeader();

            template <class T>
            PacketBuilderContent &addPayload(T payload) {
                Serialization::serialize(_ss, payload);
                return *this;
            };

            std::string get();

        private:
            std::stringstream _ss;

            PacketType _type;
            std::size_t _tick;
            std::string _uuid;
    };

    class PacketBuilder {
        public:
            PacketBuilder(std::string uuid);
            ~PacketBuilder() = default;

            PacketBuilderContent create(std::size_t tick, PacketType type);

        private:
            std::string _uuid;
    };
}
