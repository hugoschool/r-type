# Overview

This section explains how the server works.

## Using

When running the server, the port is set by default as `25000`.

## Source of truth

The server is always considered as the source of truth.

All communication **must** pass by the server. The clients must always believe the server.

## Handshake

The handshake between the client and the server is explained in the [Protocol Example](../protocol/example.md) and [Protocol Packets](../protocol/packets.md).
