# Client

## Comparative: Graphical librabries

We were searching for a graphical library that works well with C++, that doesn't need much more encapsulation, that is realitively simple to use and that can handle some accessibility funtionnalities like controller support (so that person with physical disabilities can play our game through a custom controller) or that can handle shaders (to apply color blind or high contrast shaders).

| Graphic library | C++ compatibility                 | Windows compatibility | Simplicity of usage | Controller support | Shaders support       |
| --------------- | --------------------------------- | --------------------- | ------------------- | ------------------ | --------------------- |
| Raylib          | 🟡 A binding for cpp already exist | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders |
| SFML            | 🟢 No encapsulation requiered      | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders |
| SDL             | 🔴 Encapsulation requiered         | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders |

We chose SFML after creating this comparation table since it is the one that suits our requierments the more.

## How does the client-server communication work

    1. At the handshake the server sends the client a copy of the ECS.
    2. Each tick the client will send the player inputs to the server.
    3. The server will update and track all the changes in the ECS.
    4. The server will send the all the tracked changes to the client.

## What does the client actualy do

The client takes the player input send it to the server. While the server is processing that information, the client client create predictions about how the server would react (based on a snapnshot system, one snapshot per tick). When the client get the server response, it evaluates the prediction to see if it matches the server responses. If not the server is alway right and the client changes the ecs state to match the servers one.
