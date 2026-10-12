#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/asio.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

namespace rtype::serialize {

    struct PacketHeader {
        std::uint64_t id;
        size_t size;
    };

    class Serialization {
    public:
        template<typename Packet, typename Socket>
        static void serialize(Packet packet, Socket &socket, const std::uint64_t id) {
            boost::asio::streambuf buf;
            std::ostream os(&buf);
            boost::archive::text_oarchive ar(os);
            ar & packet;
            const PacketHeader header = {id, buf.size()};
            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::buffer(&header, sizeof(header)));
            buffers.push_back(buf.data());
            boost::asio::write(socket, buffers);
        }

        template<typename Socket>
        static PacketHeader deserialize(Socket &socket, boost::asio::streambuf &buf) {
            PacketHeader header;
            boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)));

            boost::asio::read(socket, buf.prepare(header.size));
            buf.commit(header.size);
            // std::cout << "read " << rc << " bytes" << std::endl;
            return header;
        }
    };
}
