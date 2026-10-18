#include "protocols/AProtocol.hpp"
#include "ClientHandler.hpp"

rtype::server::AProtocol::AProtocol(boost::asio::io_context &io_context, std::uint16_t port,
    SafeMessageQueue &SafeMessageQueue, ClientHandler &clientHandler)
    : _io_context(io_context), _port(port), _messageQueue(SafeMessageQueue), _clientHandler(clientHandler) {
}
