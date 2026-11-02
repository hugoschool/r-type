#pragma once

#include <packets.pb.h>

namespace fengine::ecs {
    struct EntityTypeComponent {
        public:
            rtype::packets::EntityType type;
    };
}
