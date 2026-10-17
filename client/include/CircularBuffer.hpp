#pragma once

#include <cstddef>
#include <vector>

namespace rtype::client {
    template <typename T>
    class CircularBuffer {
        public:
            CircularBuffer(std::size_t size) : _size(size), _buffer(), _writerIndex(0), _readerIndex(0) {
            }

            ~CircularBuffer() = default;

            T getElem() {
                T elem = _buffer.at(_readerIndex);

                _readerIndex = (_readerIndex + 1) % _size;
                return elem;
            }

            void addElement(T elem) {
                if (_buffer.size() < _size) {
                    _buffer.push_back(elem);
                } else {
                    _buffer[_writerIndex] = elem;
                }
                _writerIndex = (_writerIndex + 1) % _size;
            }

            bool canRead() const {
                return _writerIndex != _readerIndex;
            }

        private:
            size_t _size;
            std::vector<T> _buffer;
            int _writerIndex;
            int _readerIndex;
    };
}
