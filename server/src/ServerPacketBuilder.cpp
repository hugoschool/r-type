#include "ServerPacketBuilder.hpp"
#include "ClientHandler.hpp"
#include "TickHandler.hpp"
#include "r-type/packets/PacketBuilder.hpp"

rtype::server::ServerPacketBuilder::ServerPacketBuilder(rtype::server::TickHandler &tick)
    : packets::PacketBuilder(DUMMY_UUID), _tick(tick) {
}

rtype::packets::PacketBuilderContent rtype::server::ServerPacketBuilder::create(rtype::packets::PacketType type) {
    return packets::PacketBuilder::create(_tick.getTick(), type);
}
