#pragma once

#include "AProtocol.hpp"
#include "CircularBuffer.hpp"
#include <array>
namespace rtype::client {
    class UDPProtocol : public AProtocol {
            using udp = boost::asio::ip::udp;
            using EventKey = fengine::modules::display::EventKey;

        public:
            UDPProtocol(rtype::client::CircularBuffer<EventKey> &input);
            ~UDPProtocol() override;
            void connect(char *av[]) override;
            void sendMsg() override;
            std::string readMsg(boost::system::error_code &error) override;
            void setSocketNonBlocking() override;
            void close() override;

        private:
            udp::resolver _resolver;
            udp::endpoint _endpoint;
            std::array<char, 1024> _buffer;
            udp::socket _socket;
    };
}
