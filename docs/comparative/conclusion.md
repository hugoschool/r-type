# Conclusion

In the last couple of parts, we went over a lot of technologies and dug into and compared them to see what would be the best choices for our project.

## Packaging & Build Systems

For the build system, we went with CMake. It's solid & works on Windows reliably.

As for the package manager, we went with CPM as it is native to CMake.

## ECS vs Mediator

On ECS or Mediator comparison, we went with the ECS pattern.

It seems like the most solid pattern for our game and it's well used in most game engines nowadays.

## Network library

Concerning the network library, we chose Boost Asio.
Boost is a widely recognized and used library that may help us with other parts of the code too.

Boost Asio also comes with a good documentation.

## Graphical library

We chose SFML since it has a great C++ compatibility, it's easily compatible on most platforms and has a great developer API.
