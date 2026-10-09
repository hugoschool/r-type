#pragma once

#include <exception>
#include <string>

namespace rtype::client {
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

    class ClientException : public Exception {
        public:
            ClientException(std::string msg) noexcept : Exception(msg) {};
            ~ClientException() noexcept {};
    };
}
