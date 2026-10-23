#pragma once

#include "IProtocol.hpp"
#include <array>
#include <boost/asio/io_context.hpp>

namespace fnetwork::client {
    class AProtocol : public IProtocol {
        public:
            AProtocol(boost::asio::io_context &_io_context, std::string host, std::uint16_t port);
            ~AProtocol() override = default;

        protected:
            boost::asio::io_context &_io_context;

            std::string _host;
            std::uint16_t _port;

            std::array<char, 1024> _buffer;
    };
}
