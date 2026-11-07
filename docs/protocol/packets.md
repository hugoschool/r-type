# Packets

Explanation of most packets and what they do.

## Overview

Here's a quick overview of a few rules to follow to use the R-Type protocol.

- Packets **must always** be sent using little endian.
- Packets **must always start** with a [PacketHeader](#packetheader).
- Packets can contain a payload right after the PacketHeader. The payload can be of different type and size.
- A packet **can only contain at most** one payload.
- String must always be null terminated (`\0`)

Below, you will find everything that needs to be contained within a packet inside the r-type game.

Each packet will have a table like the one below.
The content section represents what is being sent, the size section is the size of the field in bytes and the type section represents one of 4 types: (string, number, floating point number, bool).

| Content        | Size | Type                  |
| -------------- | ---- | --------------------- |
| Type           | 1    | bool                  |
| Test bytes     | 4    | number                |
| Test bytes 2   | 8    | floating point number |
| Example string | 7    | string                |

## Types

Here are all of the different types that can be sent in the PacketHeader with a quick explanation:

| Type                 | Number | Explanation                                                                                           | Payload                      |
| -------------------- | ------ | ----------------------------------------------------------------------------------------------------- | ---------------------------- |
| LoginRequest         | 0      | Client asks to be logged into the server                                                              | None                         |
| LoginAnswer          | 1      | Server responds that the connection was successful                                                    | [Yes](#loginanswer)          |
| LoginFailed          | 2      | Server responds that the connection is impossible                                                     | None                         |
| EntitySpawn          | 3      | An entity has spawned                                                                                 | [Yes](#entityspawn)          |
| EntityMove           | 4      | An entity has moved (coordinates)                                                                     | [Yes](#entitymove)           |
| EntityVelocityChange | 5      | An entity has changed velocity                                                                        | [Yes](#entityvelocitychange) |
| EntityDied           | 6      | An entity has died                                                                                    | [Yes](#entitydied)           |
| Ack                  | 7      | Client tells the server it has received a packet / Server tells the client it has received its packet | None                         |
| PlayerEvent          | 8      | Player asks to perform an event                                                                       | [Yes](#playerevent)          |
| Level                | 9      | Server tells the client the new level number                                                          | [Yes](#level)                |

If a packet with an unknown type is sent, it will be ignored.

## Entity types

Entity has different types, here's all of them:

- PLAYERS   (0)
- BULLETS   (1)
- E_RUNNER  (2): Different enemy types
- E_SHOOTER (3): Different enemy types
- E_BOSS    (4): Different enemy types
- BONUS     (5): Power ups

## Player events types

- PLAY (0)
- UP (1)
- LEFT (2)
- DOWN (3)
- RIGHT (4)
- SHOOT (5)

## Priority

A packet can have a priority. It must be directly set in the header.

The current two states of priority are:
```
0 = HIGH
1 = LOW
```

## Handshake

You must perform a handshake with the server to be set to a "connected" state.

The request is done via sending a packet, only containing a header with the type `LoginRequest`.

The response is done via the server sending a packet, containing a header with the type `LoginAnswer` and a [payload](#loginanswer).

The server might respond with a `LoginFailed` in case it has already enough players.

## PacketHeader

First off, the packet **must** always contain a header.

It contains the type of the packet to be sent, the size of the payload and a player UUID.

The size of the payload may be 0 if this packet type has no payload and the player UUID may be empty if it is a LoginRequest packet.

| Content                             | Size | Type   |
| ----------------------------------- | ---- | ------ |
| [Type](#types)                      | 4    | number |
| Size of the following payload       | 8    | number |
| Current tick the packet was sent on | 8    | number |
| Player's UUID                       | 37   | string |
| [The packet's priority](#priority)  | 4    | number |

## Payloads

### LoginAnswer

Constitutes the second half of the [handshake](#handshake).

| Content       | Size | Type   |
| ------------- | ---- | ------ |
| Player's UUID | 37   | string |

## EntitySpawn

Sent from the server to the client if a new entity has spawned.

| Content                      | Size | Type                  |
| ---------------------------- | ---- | --------------------- |
| Entity ID                    | 8    | number                |
| [Entity Type](#entity-types) | 4    | number                |
| Width of its hitbox          | 4    | floating point number |
| Height of its hitbox         | 4    | floating point number |

## EntityMove

Sent from the server to the client if a new entity has moved.

| Content   | Size | Type                  |
| --------- | ---- | --------------------- |
| Entity ID | 8    | number                |
| X         | 8    | floating point number |
| Y         | 8    | floating point number |

## EntityVelocityChange

Sent from the server to the client if a new entity has had a velocity change.

| Content   | Size | Type                  |
| --------- | ---- | --------------------- |
| Entity ID | 8    | number                |
| X         | 8    | floating point number |
| Y         | 8    | floating point number |

## EntityDied

Tells the players that an entity has died.

| Content   | Size | Type   |
| --------- | ---- | ------ |
| Entity ID | 8    | number |

## PlayerEvent

Sends a player event.

| Content                       | Size | Type   |
| ----------------------------- | ---- | ------ |
| [Event](#player-events-types) | 4    | number |

## Level

Sends the current level.

| Content | Size | Type   |
| ------- | ---- | ------ |
| Level   | 8    | number |
