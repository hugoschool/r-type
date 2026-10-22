#pragma once

namespace fnetwork::server {
    class IProtocol {
        public:
            virtual ~IProtocol() = default;

            virtual void run() = 0;
    };
}
