#include "protocols/AProtocol.hpp"

rtype::server::AProtocol::AProtocol(boost::asio::io_context &io_context, std::uint16_t port,
    SafeMessageQueue &SafeMessageQueue, ClientMessageQueueHandler &clientMQHandler)
    : _io_context(io_context), _port(port), _messageQueue(SafeMessageQueue), _clientMQHandler(clientMQHandler) {
}
