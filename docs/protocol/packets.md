# Packets

Explanation of most packets and what they do.

## Header

First off, the packet must always contain a header.

It contains the type of the packet to be sent, the size of the payload and a player UUID.

The size of the payload may be 0 if this packet type has no payload and the player UUID may be empty if it is a LoginRequest packet.

## LoginRequest

Constitutes half of the handshake.

If sent without player UUID inside the packet header, creates a new client to be connected to the server.

> Not implemented yet.
>
> If sent with a player UUID, if this player is disconnecting / in a reconnecting state, it reconnects the player.

Packet contains no payload.

## LoginAnswer

Constitutes the second half of the handshake.

Packet contains a payload (LoginAnswer) which sends the player UUID.

## LoginFailed

This packet may be sent if there are already too many player connected to the server.

Packet contains no payload.

## EntitySpawn

Sent from the server to the client if a new entity has spawned.

Packet contains a payload (EntitySpawn, which is an EntityIdOnly payload) which sends the entity ID.

## EntityMove

Sent from the server to the client if a new entity has moved.

Packet contains a payload (EntityMove) which sends the new positions of the entity.

## EntityVelocityChange

Sent from the server to the client if a new entity has had a velocity change.

Packet contains a payload (EntityVelocityChange) which sends the new velocity.

## EntityDied

Tells the players that an entity has died.

Packet contains a payload (EntityDied, which is an EntityIdOnly payload) which sends the entity ID.

## Ack

Packet contains no payload.

## InputPlayer

Sends a client input.

The server will process this player input and turns it into actions in the game (collect something, move, etc...)

Packet contains a payload which sends the input.
