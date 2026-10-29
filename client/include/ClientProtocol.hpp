#pragma once

#include "CircularBuffer.hpp"
#include "boost/system/detail/error_code.hpp"
#include "r-type/event/PlayerEvent.hpp"
#include <boost/asio/io_context.hpp>
#include <cstddef>
#include <f-engine/modules/display/events/EventKey.hpp>
#include <f-network/client/UDPProtocol.hpp>

namespace rtype::client {
    class ClientProtocol {
        public:
            using PlayerEvent = rtype::event::Event;

            ClientProtocol(rtype::client::CircularBuffer<PlayerEvent> &inputs, std::string host, std::string port,
                std::size_t &tick);
            ~ClientProtocol();

            void sendConnectionPackets();

            std::string read(boost::system::error_code &error);
            void sendMessage();

            void setSocketNonBlocking();

        private:
            rtype::client::CircularBuffer<PlayerEvent> &_inputs;
            boost::asio::io_context _io_context;
            fnetwork::client::UDPProtocol _udp;

            std::string _uuid;
            std::size_t &_tick;
    };
}
