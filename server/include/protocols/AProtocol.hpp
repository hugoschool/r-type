#pragma once

#include "TickHandler.hpp"
#include "protocols/IProtocol.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/lockfree/queue.hpp>
#include <cstdint>

namespace rtype::server {
    class AProtocol : public IProtocol {
        public:
            AProtocol(boost::asio::io_context &io_context, std::uint16_t port, TickHandler::MessageQueue &messageQueue);
            ~AProtocol() override = default;

        protected:
            boost::asio::io_context &_io_context;
            std::uint16_t _port;
            TickHandler::MessageQueue &_messageQueue;
    };
}
