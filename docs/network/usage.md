# Usage

This document describes the usage of the network library.

## Client

Import and construct the wanted protocol inside your class.

Example for UDP protocol:

```cpp
boost::asio::io_context io_context;
UDPProtocol udp(io_context, "hello", "12345");
udp.send("hello world");
```

## Server

For the UDP protocol, derive from the `UDPProtocol` class and implement the `handleReceive` method to answer to the client:

```cpp
class ServerProtocol : public fnetwork::server::async::UDPProtocol {
    public:
        ServerProtocol(boost::asio::io_context &io_context, std::uint16_t port) : fnetwork::server::async::UDPProtocol(io_context, port) {
        }
        ~ServerProtocol() override = default;

        void handleReceive(const boost::system::error_code &error, std::size_t) final {
            if (!error) {
                std::string initialMessage = "hello world";
                std::shared_ptr<std::string> message = std::make_shared<std::string>(initialMessage);

                _socket.async_send_to(boost::asio::buffer(*message), _endpoint,
                    std::bind(&UDPProtocol::handleSend, this, message, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            }

            // The function MUST always be called to accept new connections
            startReceive();
        }
};
```
