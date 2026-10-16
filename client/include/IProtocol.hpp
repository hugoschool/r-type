#pragma once

#include "boost/system/detail/error_code.hpp"
namespace rtype::client {
    class IProtocol {
        public:
            virtual ~IProtocol() = default;
            virtual void connect(char *av[]) = 0;
            virtual void sendMsg() = 0;
            virtual void readMsg(boost::system::error_code &error) = 0;
            virtual void setSocketNonBlocking() = 0;
            virtual void close() = 0;
    };
}
