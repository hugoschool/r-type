# Architecture

This document represents the architecture of the Server part of the r-type.

```mermaid
classDiagram

Server --> SubscriberHandler
class World["World (Comes from Engine)"]
Server --> World
Server --> TickHandler
Server --> ClientHandler
Server --> ServerProtocol
Server --> ServerPacketBuilder
Server --> PriorityHandler

ClientHandler --> ClientMessageQueueHandler

class UDPProtocol["UDPProtocol (Comes from Network)"]
ServerProtocol <|-- UDPProtocol
```

## Explanation

`SubscriberHandler` allows us to get the events from the engine (specifically, the ECS, which holds all systems) and then send them to the clients. (see [Subscribers document in Engine](../engine/subscribers.md))

`ClientMessageQueueHandler` is what allows us to send the messages back to the client. We queue the messages to be sent at the end of the tick back to the client.
