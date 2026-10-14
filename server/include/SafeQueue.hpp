#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace rtype::server {
    template <typename T>
    class SafeQueue {
        public:
            void push(T item) {
                std::unique_lock<std::mutex> lock(_mutex);
                _queue.push(item);
                _cv.notify_one();
            }

            T pop() {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this]() { return !_queue.empty(); });

                T item = _queue.front();
                _queue.pop();

                return item;
            }

            bool empty() {
                std::unique_lock<std::mutex> lock(_mutex);

                return _queue.empty();
            }

        private:
            std::queue<T> _queue;
            std::mutex _mutex;
            std::condition_variable _cv;
    };
}
