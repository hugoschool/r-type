#pragma once

#include "CircularBuffer.hpp"
#include "IProtocol.hpp"
#include "boost/asio/io_context.hpp"
#include "f-engine/modules/display/events/EventKey.hpp"

namespace rtype::client {
    class AProtocol : public IProtocol {
            using EventKey = fengine::modules::display::EventKey;

        protected:
            boost::asio::io_context _io_context;
            std::uint16_t _port;
            rtype::client::CircularBuffer<EventKey> &_inputs;

            std::string _uuid;

        public:
            AProtocol(rtype::client::CircularBuffer<EventKey> &input)
                : _io_context(), _port(0), _inputs(input), _uuid() {
            }
            ~AProtocol() override = default;
    };
}
