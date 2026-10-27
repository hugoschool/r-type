#pragma once

#include "r-type/packets/Packet.hpp"
#include "r-type/packets/Serialization.hpp"
#include <sstream>
#include <string>

namespace rtype::packets {
    class PacketBuilderContent {
        public:
            PacketBuilderContent(PacketType type, std::string uuid);
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
            std::string _uuid;
    };

    class PacketBuilder {
        public:
            PacketBuilder(std::string uuid);
            ~PacketBuilder() = default;

            PacketBuilderContent create(PacketType type);

        private:
            std::string _uuid;
    };
}
