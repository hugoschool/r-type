#include "ClientPacketBuilder.hpp"
#include "r-type/packets/PacketBuilder.hpp"

rtype::client::ClientPacketBuilder::ClientPacketBuilder() : packets::PacketBuilder("") {
}

rtype::packets::PacketBuilderContent rtype::client::ClientPacketBuilder::create(packets::PacketType type) {
    return packets::PacketBuilder::create(0, type);
}
