#pragma once

namespace rtype::packets {
    enum class PacketType {
        UpdatePlayer,
        UpdateEntity,
        KillPlayer,
        KillEntity,
        AddPlayer,
        AddEntity,
        DeadPlayer,
        Ack,
    };
}
