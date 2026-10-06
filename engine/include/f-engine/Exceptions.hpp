#pragma once

#include <exception>
#include <string>

namespace fengine {
    class Exception : public std::exception {
        private:
            std::string _msg;
        public:
            Exception(std::string msg) noexcept : _msg(msg) {};
            ~Exception() noexcept {};

            const char *what() const noexcept override {
                return _msg.c_str();
            };
    };

    class ECSException : public Exception {
        public:
            ECSException(std::string msg) noexcept : Exception(msg) {};
            ~ECSException() noexcept {};
    };

    class DisplayException : public Exception {
        public:
            DisplayException(std::string msg) noexcept : Exception(msg) {};
            ~DisplayException() noexcept {};
    };

    class SfmlException : public DisplayException {
        public:
            SfmlException(std::string msg) noexcept : DisplayException(msg) {};
            ~SfmlException() noexcept {};
    };
}
