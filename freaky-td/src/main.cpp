#include "HelloWorld.hpp"
#include <f-engine/ecs/Registry.hpp>

int main(void) {
    fengine::ecs::Registry reg = fengine::ecs::Registry();

    freakytd::HelloWorld();
    return 0;
}
