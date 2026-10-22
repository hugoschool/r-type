#pragma once

#include <boost/system/detail/error_code.hpp>

namespace fnetwork::client {
    class IProtocol {
        public:
            virtual ~IProtocol() = default;

            virtual void connect() = 0;
            virtual void close() = 0;

            virtual void send(std::string content) = 0;
            virtual std::string read(boost::system::error_code &error) = 0;
            virtual void setSocketNonBlocking() = 0;
    };
}
