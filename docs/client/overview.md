# Overview

## What does the client actually do

The client takes the player input send it to the server.
While the server is processing that information, the client creates predictions about how the server would react (based on a snapshot system, one snapshot per tick).
When the client get the server response, it evaluates the prediction to see if it matches the server responses.
If not, the server is always right (source of truth) and the client changes the ECS state to match the server one.

The server is running on 20 ticks per seconds, however the client is ideally having 60 fps that is not related to the server actualization rate.
This is permitting us to have more tries on getting the current snapshot actualisation.

## How does the client-server communication work

1. At the handshake the server sends the client a "copy" of the ECS.
2. Each tick, if needed, the client sends the player inputs to the server.
3. The server updates and track all the changes in the ECS.
4. The server sends the all the tracked changes to the client.

<!-- Interpolation Client-side prediction -->
