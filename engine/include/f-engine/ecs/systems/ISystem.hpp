#pragma once

namespace fengine {
    namespace ecs {
        class Registry;

        class ISystem {
            public:
                virtual ~ISystem() = default;

                virtual void update(Registry &) = 0;
        };
    }
}
