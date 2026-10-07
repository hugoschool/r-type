#pragma once

namespace rtype::server {
    class IProtocol {
        public:
            virtual ~IProtocol() = default;

            virtual void run() = 0;
    };
}
