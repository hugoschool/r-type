#include "ServerPacketBuilder.hpp"
#include "ClientHandler.hpp"
#include "r-type/packets/PacketBuilder.hpp"

rtype::server::ServerPacketBuilder::ServerPacketBuilder() : packets::PacketBuilder(DUMMY_UUID) {
}
