#include "ServerPacketBuilder.hpp"
#include "ClientHandler.hpp"
#include "r-type/packets/PacketBuilder.hpp"

rtype::server::ServerPacketBuilder::ServerPacketBuilder(std::size_t &tick) : packets::PacketBuilder(DUMMY_UUID), _tick(tick) {
}

rtype::packets::PacketBuilderContent rtype::server::ServerPacketBuilder::create(rtype::packets::PacketType type) {
    return packets::PacketBuilder::create(_tick, type);
}
