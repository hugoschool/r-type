#pragma once

#include "SafeQueue.hpp"
#include <chrono>
#include <cstdint>

namespace rtype::server {
    constexpr std::uint64_t TICKS_PER_SECOND = 20;
    constexpr std::uint64_t MAX_MESSAGE_PER_TICK = 2048;

    class TickHandler {
        public:
            using MessageQueue = SafeQueue<std::string>;

            explicit TickHandler();
            ~TickHandler() = default;

            // Checks if a tick has passed
            // Returns true if it is the case
            // Returns false if it still hasn't passed yet
            //
            // Resets the tick once it has passed
            bool tickPassed();

            MessageQueue &getMessageQueue();

        private:
            std::chrono::steady_clock::time_point _clock;

            const std::chrono::milliseconds _delayBetweenTicks;

            MessageQueue _messageQueue;
    };
}
