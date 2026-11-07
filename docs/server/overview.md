# Overview

This section explains how the server works.

## Using

When running the server, the port is set by default as `25000`.

You can set the port when running the server, in the first argument:

```sh
./build/server/r-type_server        # Runs on port 25000
./build/server/r-type_server 12345  # Runs on port 12345
```

## Source of truth

The server is always considered as the source of truth.

All communication **must** pass by the server. The clients must always believe the server.

## Handshake

The handshake between the client and the server is explained in the [Protocol Example](../protocol/example.md) and [Protocol Packets](../protocol/packets.md).
