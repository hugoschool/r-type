#include "TickHandler.hpp"
#include <chrono>
#include <cstddef>

rtype::server::TickCountHolder::TickCountHolder() : _mutex(), _tick(0) {
}

void rtype::server::TickCountHolder::update() {
    std::unique_lock lock(_mutex);

    _tick += 1;
}

std::size_t rtype::server::TickCountHolder::get() {
    std::unique_lock lock(_mutex);

    return _tick;
}

rtype::server::TickHandler::TickHandler()
    : _clock(), _delayBetweenTicks(1 * 1000 / TICKS_PER_SECOND), _messageQueue(), _tick() {
}

bool rtype::server::TickHandler::tickPassed() {
    auto currentTime = std::chrono::steady_clock::now();
    auto msDuration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _clock);

    if (msDuration >= _delayBetweenTicks) {
        _clock = currentTime;
        _tick.update();
        return true;
    }
    return false;
}

rtype::server::SafeMessageQueue &rtype::server::TickHandler::getMessageQueue() {
    return _messageQueue;
}

std::size_t rtype::server::TickHandler::getTick() {
    return _tick.get();
}
