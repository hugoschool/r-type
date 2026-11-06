#include "ServerPacketBuilder.hpp"
#include "ClientHandler.hpp"
#include "TickHandler.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/PacketBuilder.hpp"
#include <cstddef>
#include <cstdint>
#include <mutex>

rtype::server::ServerPacketBuilder::ServerPacketBuilder(rtype::server::TickHandler &tick)
    : packets::PacketBuilder(DUMMY_UUID), _tick(tick), _mutex(), _id(1) {
}

rtype::packets::PacketBuilderContent rtype::server::ServerPacketBuilder::create(rtype::packets::PacketType type,
    packets::PacketPriority priority, size_t id) {
    return packets::PacketBuilder::create(_tick.getTick(), type, priority, id);
}

void rtype::server::ServerPacketBuilder::incrementID() {
    std::unique_lock lock(_mutex);

    if (_id < INT32_MAX)
        _id++;
    else
        _id = 1;
}
size_t rtype::server::ServerPacketBuilder::getCurrentId() {
    size_t id = _id;

    incrementID();
    return id;
}
