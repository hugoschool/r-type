#include <r-type/packets/PacketBuilder.hpp>

rtype::packets::PacketBuilderContent::PacketBuilderContent(PacketType type, std::string uuid)
    : _ss(), _ar(_ss), _type(type), _uuid(uuid) {};

rtype::packets::PacketBuilderContent &rtype::packets::PacketBuilderContent::addHeader() {
    PacketHeader header;

    header.size = 0;
    header.type = _type;
    header.setUuid(_uuid);
    Serialization::serialize(_ar, header);

    return *this;
}

std::string rtype::packets::PacketBuilderContent::get() {
    return _ss.str();
}

rtype::packets::PacketBuilder::PacketBuilder(std::string uuid) : _uuid(uuid) {
}

rtype::packets::PacketBuilderContent rtype::packets::PacketBuilder::create(rtype::packets::PacketType type) {
    return PacketBuilderContent(type, _uuid);
}
