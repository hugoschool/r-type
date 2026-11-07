# Building

This document describes how to build, and link with the engine inside of your own game.

## CMake compilation

To use the engine within CMake, you can import the `r-type` as you would with any other CMake project.

Make sure to use the `ONLY_BUILD_ENGINE ON` option as to only compile the game engine.

### CPM

```cmake
# Currently pinned to the ffc3f40e06490ccc63b2b9fa09bf0fa0b8d87607 commit
# You can replace it to target a certain version (ex: v1.0.0)
CPMAddPackage(
    NAME f-engine
    URL https://github.com/hugoschool/r-type/archive/ffc3f40e06490ccc63b2b9fa09bf0fa0b8d87607.tar.gz
    OPTIONS "ONLY_BUILD_ENGINE ON"
)
```

## Linking

Once the game engine is compiled, you can link with the different parts of the game engine.

In the following example, I wish to use in my game, the Camera, the ECS, the Display module and the Asset Manager.

That means that I have to link with `freaky-engine-asset-manager freaky-engine-camera freaky-engine-ecs freaky-engine-module-display`

### CMake

```cmake
target_link_libraries(
    ${BINARY} PUBLIC
    freaky-engine-asset-manager
    freaky-engine-camera
    freaky-engine-ecs
    freaky-engine-module-display
)
```
