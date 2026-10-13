#pragma once

#include "Packet.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <ostream>

namespace rtype::packets {
    class Serialization {
        public:
            using deserialize_archive = boost::archive::binary_iarchive;
            using serialize_archive = boost::archive::binary_oarchive;

            template <class Payload>
            static void serialize(std::ostream &s, PacketHeader header, Payload payload) {
                serialize_archive a(s);

                a & header;
                a & payload;
            }

            static deserialize_archive createDeserializeArchive(std::istream &s) {
                return deserialize_archive(s);
            }

            template <class T>
            static void deserialize(deserialize_archive &a, T &content) {
                a & content;
            }
    };
}
