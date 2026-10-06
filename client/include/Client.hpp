#pragma once

#include "ClientHelper.hpp"

namespace rtype {
    namespace client {
        class Client {
            private:
                int connect(char *av[]);
                int loop();

            public:
                Client();
                ~Client();

                ClientHelper helper;

                int run(char *av[]);
        };
    }
}
