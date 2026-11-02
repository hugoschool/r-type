#pragma once

#include <r-type/proto/packets.pb.h>

namespace fengine::ecs {
    struct EntityTypeComponent {
        public:
            rtype::packets::EntityType type;
    };
}
