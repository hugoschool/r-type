#pragma once

#include <exception>
#include <string>

namespace freakytd {
    class Exception : public std::exception {
        public:
            Exception(std::string message) noexcept : _message(message) {};
            ~Exception() noexcept override = default;

            const char *what() const noexcept override {
                return _message.c_str();
            };

        private:
            std::string _message;
    };

    class LevelException : public Exception {
        public:
            LevelException(std::string message) noexcept : Exception(message) {};
            ~LevelException() noexcept override = default;
    };
}
