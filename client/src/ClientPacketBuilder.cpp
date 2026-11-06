#include "ClientPacketBuilder.hpp"
#include "r-type/packets/Packet.hpp"
#include "r-type/packets/PacketBuilder.hpp"
#include <cstddef>

rtype::client::ClientPacketBuilder::ClientPacketBuilder() : packets::PacketBuilder("") {
}

rtype::packets::PacketBuilderContent rtype::client::ClientPacketBuilder::create(packets::PacketType type,
    packets::PacketPriority priority, size_t id) {
    return packets::PacketBuilder::create(0, type, priority, id);
}
