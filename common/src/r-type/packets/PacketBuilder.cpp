#include "r-type/packets/Packet.hpp"
#include <cstddef>
#include <r-type/packets/PacketBuilder.hpp>

rtype::packets::PacketBuilderContent::PacketBuilderContent(PacketType type, std::size_t tick, std::string uuid,
    PacketPriority priority, size_t id)
    : _ss(), _ar(_ss), _type(type), _tick(tick), _uuid(uuid), _priority(priority), _id(id) {};

rtype::packets::PacketBuilderContent &rtype::packets::PacketBuilderContent::addHeader() {
    PacketHeader header;

    header.size = 0;
    header.type = _type;
    header.tick = _tick;
    header.setUuid(_uuid);
    header.priority = _priority;
    header.id = _id;
    Serialization::serialize(_ar, header);

    return *this;
}

std::string rtype::packets::PacketBuilderContent::get() {
    return _ss.str();
}

rtype::packets::PacketBuilder::PacketBuilder(std::string uuid) : _uuid(uuid) {
}

rtype::packets::PacketBuilderContent rtype::packets::PacketBuilder::create(std::size_t tick,
    rtype::packets::PacketType type, PacketPriority priority, size_t id) {
    return PacketBuilderContent(type, tick, _uuid, priority, id);
}

void rtype::packets::PacketBuilder::setUuid(std::string uuid) {
    _uuid = uuid;
}
