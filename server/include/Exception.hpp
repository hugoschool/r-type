#pragma once

#include <exception>
#include <string>

namespace rtype::server {
    class Exception : public std::exception {
        private:
            std::string _msg;

        public:
            Exception(std::string msg) noexcept : _msg(msg) {};
            ~Exception() noexcept override {};

            const char *what() const noexcept override {
                return _msg.c_str();
            };
    };

    class ServerException : public Exception {
        public:
            ServerException(std::string msg) noexcept : Exception(msg) {};
            ~ServerException() noexcept override {};
    };
}
