#pragma once

#include "ClientHelper.hpp"

namespace rtype {
    namespace client {
        class Client {
            private:
            public:
                Client();
                ~Client();

                ClientHelper helper;
        };
    }
}
