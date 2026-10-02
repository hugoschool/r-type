# Packaging

Packaging and Build Systems is very important to build a cross platform application.

## Build System

| Name      | Cross platform                                                                                  | Maturity                                                                           | Package Manager compat                  |
| --------- | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | --------------------------------------- |
| Makefiles | 🔴 Differences between BSD, Linux. Not native on Windows                                         | Well mature (1988), used in lots of different projects                             | None, libs shall be build               |
| CMake     | 🟢 Natively built for Linux & Windows                                                            | Release in 2000, much used in C++ projects                                         | Multiple package managers are available |
| Xmake     | 🟢 Natively built for Linux & Windows                                                            | Released in 2015, hasn't found much use yet, but is rising quickly                 | Xrepo for package manager               |
| Meson     | 🟠 Built for Linux & Windows but no real official support for Windows (runs mostly on Unix-like) | Released in 2013, uses Ninja as the backend. GNOME, FD.org & more use mainly Meson | Uses subprojects                        |

After evaluating all build systems, we have decided to go with CMake for its Linux & Windows compatibility and the extensive packages support.

Multiple package managers exist on CMake, we still need to evaluate the different options.

## CMake package manager

| Name  | Usability                                                              | Package availability          |
| ----- | ---------------------------------------------------------------------- | ----------------------------- |
| CPM   | 🟢 Native to CMake                                                      | 🟢 Wrapper around FetchContent |
| Vcpkg | 🟠 Required a Git submodule                                             | 🟢 Over 2300 libraries         |
| Conan | 🔴 Requires a separate Python app (installed by system package manager) | 🟠 Over 1900 libraries         |

CPM seemed like the best option, both simple and integrated directly into CMake.

## Conclusion

CMake & CPM seems like the most simple choice for a simple setup. One concern is caching in CI builds.
