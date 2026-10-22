#pragma once

namespace fnetwork::server::async {
    class IProtocol {
        public:
            virtual ~IProtocol() = default;

            virtual void run() = 0;
    };
}
