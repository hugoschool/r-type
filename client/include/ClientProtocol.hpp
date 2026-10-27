#pragma once

#include "CircularBuffer.hpp"
#include "boost/system/detail/error_code.hpp"
#include <boost/asio/io_context.hpp>
#include <f-engine/modules/display/events/EventKey.hpp>
#include <f-network/client/UDPProtocol.hpp>

namespace rtype::client {
    class ClientProtocol {
        public:
            using EventKey = fengine::modules::display::EventKey;

            ClientProtocol(rtype::client::CircularBuffer<EventKey> &inputs, std::string host, std::string port);
            ~ClientProtocol();

            void sendConnectionPackets();

            std::string read(boost::system::error_code &error);
            void sendMessage();

            void setSocketNonBlocking();
            std::optional<std::size_t> &getEntityId();

        private:
            rtype::client::CircularBuffer<EventKey> &_inputs;
            boost::asio::io_context _io_context;
            fnetwork::client::UDPProtocol _udp;

            std::optional<std::size_t> _entityId;
            std::string _uuid;
    };
}
