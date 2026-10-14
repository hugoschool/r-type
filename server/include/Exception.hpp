#pragma once

#include <exception>
#include <string>

namespace rtype::server {
    class Exception : public std::exception {
        public:
            Exception() = delete;
            explicit Exception(std::string msg) noexcept : _msg(msg) {};
            ~Exception() override = default;

            const char *what() const noexcept override {
                return _msg.data();
            };

        protected:
            std::string _msg;
    };
}
