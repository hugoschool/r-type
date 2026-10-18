# Communication example

This document provides a simple communication example between the server and the client.

```mermaid
sequenceDiagram
    Client ->> Server: LoginRequest
    Server ->> Client: LoginAnswer (contains the client's assigned UUID)
    Note over Server,Client: From now on, the client must always send his UUID in the packet header
    Note over Server,Client: Right after logging in, the server will queue the entities in the message queue. (Explained below)
    Client ->> Server: Acknowledgment
    Server ->> Client: Acknowledgment
    Server ->> Client: If any packets in queue, send all of them
    Client ->> Server: Input
    Server ->> Client: If any packets in queue, send all of them
```

## Explanation

The first part (`LoginRequest` & `LoginAnswer`) constitutes the handshake and therefore logs in the client and creates a client inside the server if needed.

After the client logs in, the server queues up all entities to be read by the client.
