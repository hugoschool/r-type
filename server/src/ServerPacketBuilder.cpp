#include "ServerPacketBuilder.hpp"
#include "ClientHandler.hpp"
#include "TickHandler.hpp"
#include "r-type/packets/PacketBuilder.hpp"
#include <cstddef>
#include <mutex>

rtype::server::ServerPacketBuilder::ServerPacketBuilder(rtype::server::TickHandler &tick)
    : packets::PacketBuilder(DUMMY_UUID), _tick(tick), _mutex(), _id(0) {
}

rtype::packets::PacketBuilderContent rtype::server::ServerPacketBuilder::create(rtype::packets::PacketType type) {
    return packets::PacketBuilder::create(_tick.getTick(), type);
}

void rtype::server::ServerPacketBuilder::incrementID() {
    std::unique_lock lock(_mutex);

    _id++;
}
size_t rtype::server::ServerPacketBuilder::getCurrentId() {
    size_t id = _id;

    incrementID();
    return id;
}
