#pragma once

#include <google/protobuf/message.h>
#include <iostream>
#include <ostream>

namespace rtype::packets {
    class Serialization {
        public:
            using deserialize_archive = std::istream;
            using serialize_archive = std::ostream;

            static serialize_archive &createSerializeArchive(std::ostream &s) {
                return s;
            }

            static void serialize(serialize_archive &a, google::protobuf::Message &content) {
                static_cast<void>(content.SerializeToOstream(&a));
            }

            static deserialize_archive &createDeserializeArchive(std::istream &s) {
                return s;
            }

            static void deserialize(deserialize_archive &a, google::protobuf::Message &content) {
                static_cast<void>(content.ParseFromIstream(&a));
            }
    };
}
