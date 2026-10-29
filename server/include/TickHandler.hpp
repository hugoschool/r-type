#pragma once

#include "SafeQueue.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>

namespace rtype::server {
    constexpr std::uint64_t TICKS_PER_SECOND = 60;
    constexpr std::uint64_t MAX_MESSAGE_PER_TICK = 2048;

    class TickCountHolder {
        public:
            TickCountHolder();
            ~TickCountHolder() = default;

            void update();
            std::size_t get();

        private:
            std::mutex _mutex;
            std::size_t _tick;
    };

    class TickHandler {
        public:
            explicit TickHandler();
            ~TickHandler() = default;

            // Checks if a tick has passed
            // Returns true if it is the case
            // Returns false if it still hasn't passed yet
            //
            // Resets the tick once it has passed
            bool tickPassed();

            SafeMessageQueue &getMessageQueue();

            std::size_t getTick();

        private:
            std::chrono::steady_clock::time_point _clock;

            const std::chrono::milliseconds _delayBetweenTicks;

            SafeMessageQueue _messageQueue;

            TickCountHolder _tick;
    };
}
