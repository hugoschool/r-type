#pragma once

#include <google/protobuf/message.h>
#include <packets.pb.h>
#include <sstream>
#include <string>

namespace rtype::packets {
    class PacketBuilderContent {
        public:
            PacketBuilderContent(PacketType type, std::size_t tick, std::string uuid);
            ~PacketBuilderContent() = default;

            PacketBuilderContent &addHeader();
            PacketBuilderContent &addPayload(google::protobuf::Message *payload);

            std::string get();

        private:
            std::stringstream _ss;

            PacketType _type;
            std::size_t _tick;
            std::string _uuid;

            PacketHeader _header;
            google::protobuf::Message *_payload;
    };

    class PacketBuilder {
        public:
            PacketBuilder(std::string uuid);
            ~PacketBuilder() = default;

            PacketBuilderContent create(std::size_t tick, PacketType type);
            void setUuid(std::string uuid);

        private:
            std::string _uuid;
    };
}
