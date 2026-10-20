#pragma once

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <ostream>

namespace rtype::packets {
    class Serialization {
        public:
            using deserialize_archive = boost::archive::binary_iarchive;
            using serialize_archive = boost::archive::binary_oarchive;

            static serialize_archive createSerializeArchive(std::ostream &s) {
                return serialize_archive(s);
            }

            template <class T>
            static void serialize(serialize_archive &a, T &content) {
                a & content;
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
