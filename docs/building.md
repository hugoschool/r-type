# Building

This project is C++20, uses CMake and CPM as a package manager.

Below is an informative guide on how to build it.

## Tools

Tools you will need:
- C++ compiler (clang++, g++, MSVC on windows)
- CMake

## Building the source

### Setup

You first need to setup the type of build.

Only two types of builds are possible:
- Release (default)
- Debug

Linux:
```sh
cmake -B build . -DCMAKE_BUILD_TYPE=Release
```

Windows:

On Windows, the config build type happens on the build step, not the configuration step.
```pwsh
cmake -B build .
cmake --build build --config Release
```

### Parallel building

On Linux (unknown on windows), you can set the amount of processors to use during the build step:
```sh
cmake --build build -j16
```

Replace 16 with the amount of processors your CPU has (result of `nproc` on Linux).

### Reset

Just deleting the `build` folder works.

## Local development

### Tools

Tools needed for local development are different. Here's what we're using.

- clangd (LSP)
- clang-format (Formatting)
- clang-format (Linting)

Before pushing any code, make sure your code is validated using the steps below.

### LSP setup

For clangd, you might require a `compile_commands.json` file.
Here are the steps for CMake to generate this file:

```sh
cmake -B build . -DCMAKE_EXPORT_COMPILE_COMMANDS=true
```

Once done, you might also need to copy the file to the root of your repo: `cp build/compile_commands.json ./`

### Linting

Linting your code is very important for a C++ project.

On this project, we're using `clang-tidy`.

A complete configuration is given in the `.clang-tidy` file.

To run it on the project, you can run the script `./scripts/run-clang-tidy`.

### Formatting

Formatting is done via `clang-format`.

A complete configuration is given in the `.clang-format` file.

To run it on the project, you can run the script `./scripts/run-clang-format`.
