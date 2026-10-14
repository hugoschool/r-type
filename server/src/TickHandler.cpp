#include "TickHandler.hpp"
#include <chrono>

rtype::server::TickHandler::TickHandler() : _clock(), _delayBetweenTicks(1 * 1000 / TICKS_PER_SECOND), _messageQueue() {
}

bool rtype::server::TickHandler::tickPassed() {
    auto currentTime = std::chrono::steady_clock::now();
    auto msDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _clock);

    if (msDuration >= _delayBetweenTicks) {
        _clock = currentTime;
        return true;
    }
    return false;
}

rtype::server::TickHandler::MessageQueue &rtype::server::TickHandler::getMessageQueue() {
    return _messageQueue;
}
