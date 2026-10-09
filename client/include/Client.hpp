#pragma once

#include "ClientHelper.hpp"
#include "f-engine/display_module/IDisplayModule.hpp"
#include <memory>
#include "f-engine/ecs/Registry.hpp"


namespace rtype {
    namespace client {
        class Client {
            private:
                std::unique_ptr<fengine::modules::display::IDisplayModule> _displayModule;
                fengine::ecs::Registry _registery;

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
