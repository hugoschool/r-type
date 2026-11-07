# Overview

This overview describes the vision we have for our protocol for the R-Type.

## Client Server Connection

The client is in one of three states:

- Disconnected
- Connecting
- Connected

Initially the client starts in *disconnected*.

Using these queries we implement the following logic when the server processes a **connection request (LoginRequest)** packet:

- If the server is full, reply with **connection denied (LoginFailed)**.
- If the connection request is from a new client and we have a slot free, assign the client to a free slot and respond with **connection accepted (LoginAnswer)**.
- If the sender corresponds to the address of a client that is already connected, *also* reply with **connection accepted**. This is necessary because the first response packet may not have gotten through due to packet loss. If we don’t resend this response, the client gets stuck in the *connecting* state until it times out.

Once the client hits *connected* it starts sending connection payload packets to the server. If no packets are received from the server in 10 seconds, the client times out and transitions to *connecting* (as it waits for a minute before transitioning to *disconnected*).
