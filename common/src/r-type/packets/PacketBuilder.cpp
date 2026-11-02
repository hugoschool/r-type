#include "r-type/proto/packets.pb.h"
#include <r-type/packets/PacketBuilder.hpp>

rtype::packets::PacketBuilderContent::PacketBuilderContent(PacketType type, std::size_t tick, std::string uuid)
    : _ss(), _type(type), _tick(tick), _uuid(uuid), _header(), _payload(nullptr) {};

rtype::packets::PacketBuilderContent &rtype::packets::PacketBuilderContent::addHeader() {
    // PacketHeader header;

    // header.set_type(_type);
    // header.set_tick(_tick);
    // header.set_playeruuid(_uuid);
    // _header = header;

    return *this;
}

rtype::packets::PacketBuilderContent &rtype::packets::PacketBuilderContent::addPayload(
    google::protobuf::Message *payload) {
    _payload = payload;
    return *this;
}

std::string rtype::packets::PacketBuilderContent::get() {
    Packet packet;

    std::cout << "header" << std::endl;
    auto *header = new PacketHeader();
    header->set_type(_type);
    header->set_tick(_tick);
    header->set_playeruuid(_uuid);
    packet.set_allocated_header(header);

    std::cout << _payload << std::endl;

    switch (header->type()) {
        case TypeLoginAnswer:
            packet.set_allocated_loginanswer(dynamic_cast<LoginAnswer *>(_payload));
            break;
        case TypeEntitySpawn:
            packet.set_allocated_entityspawn(dynamic_cast<EntitySpawn *>(_payload));
            break;
        case TypeEntityMove:
            packet.set_allocated_entitymove(dynamic_cast<EntityMove *>(_payload));
            break;
        case TypeEntityVelocityChange:
            packet.set_allocated_entityvelocitychange(dynamic_cast<EntityVelocityChange *>(_payload));
            break;
        case TypeEntityDied:
            packet.set_allocated_entitydied(dynamic_cast<EntityDied *>(_payload));
            break;
        case TypePlayerEvent:
            packet.set_allocated_playerevent(dynamic_cast<PlayerEvent *>(_payload));
            break;

        // No payloads
        case TypeLoginRequest:
        case TypeLoginFailed:
        case TypeAck:
        default:
            break;
    }

    static_cast<void>(packet.SerializeToOstream(&_ss));

    return _ss.str();
}

rtype::packets::PacketBuilder::PacketBuilder(std::string uuid) : _uuid(uuid) {
}

rtype::packets::PacketBuilderContent rtype::packets::PacketBuilder::create(std::size_t tick,
    rtype::packets::PacketType type) {
    return PacketBuilderContent(type, tick, _uuid);
}
