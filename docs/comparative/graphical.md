# Graphical library

We were searching for a graphical library that works well with C++, that doesn't need much more encapsulation, that is relatively simple to use, that can handle some accessibility functionnalities like controller support (so that person with physical disabilities can play our game through a custom controller) or that can handle shaders (to apply color blind or high contrast shaders).

## Comparative table

| Name   | C++ compatibility                 | Windows compatibility | Simplicity of usage | Controller support | Shaders support       | Developer documentation                                      |
| ------ | --------------------------------- | --------------------- | ------------------- | ------------------ | --------------------- | ------------------------------------------------------------ |
| Raylib | 🟡 A binding for cpp already exist | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders | 🟢 Awesome documentation, lots of examples                    |
| SFML   | 🟢 No encapsulation required       | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders | 🟡 Good but not enough examples directly in the documentation |
| SDL    | 🔴 Encapsulation required          | 🟢 Yes                 | 🟢 Simple to use     | 🟢 Good support     | 🟡 No built in shaders | 🟢 Great documentation, has a good amount of examples         |

## Conclusion

We chose SFML since it is the one that suits our requirements the most.
It has a great C++ compatibility, compatible on most platforms and is easy to use.
