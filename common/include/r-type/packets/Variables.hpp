#pragma once

#include <cstddef>

namespace rtype::packets {
    constexpr std::size_t UUID_SIZE = 36;
    constexpr char DUMMY_UUID[UUID_SIZE + 1] = "00000000-0000-4000-8000-000000000000";
}
