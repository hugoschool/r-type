# Client

## What does the client actualy do

The client takes the player input send it to the server. While the server is processing that information, the client client create predictions about how the server would react (based on a snapnshot system, one snapshot per tick). When the client get the server response, it evaluates the prediction to see if it matches the server responses. If not the server is alway right and the client changes the ECS state to match the servers one.

The server is running on 20 ticks per seconds, however the client is idealy having 60 fps that is not related to the server actualization rate. This is permiting us to have more tries on getting the current snapshot actualisation.

## How does the client-server communication work

    1. At the handshake the server sends the client a copy of the ECS.
    2. Each tick the client will send the player inputs to the server.
    3. The server will update and track all the changes in the ECS.
    4. The server will send the all the tracked changes to the client.

Interpolation
Client-side prediction