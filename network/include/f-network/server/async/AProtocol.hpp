#pragma once

#include "IProtocol.hpp"
#include <boost/asio/io_context.hpp>
#include <cstdint>

namespace fnetwork::server::async {
    class AProtocol : public IProtocol {
        public:
            AProtocol(boost::asio::io_context &io_context, std::uint16_t port);
            ~AProtocol() override = default;

        protected:
            boost::asio::io_context &_io_context;
            std::uint16_t _port;

            std::array<char, 1024> _buffer;
    };
}
