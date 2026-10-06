# Network library

A good communication comes with the usage of a good library. Since we use C++, a couple of choices are available.

The project requires UDP sockets to be used. We also need for it to be compatible on multiple platforms (Linux, Windows).

## Comparative table

| Name              | C++ version         | Documentation & examples                            | Compatibility                                                                                                 | Multithreading support            |
| ----------------- | ------------------- | --------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | --------------------------------- |
| Boost.Asio        | 🟢 C++11 and forward | 🟢 Lots of documentation, examples, and more         | 🟢 [Very compatible](https://www.boost.org/doc/libs/latest/doc/html/boost_asio/using.html)                     | 🟢 Yes                             |
| Asio (standalone) | 🟢 C++11 and forward | 🟢 Lots of documentation, examples, and more         | 🟢 [Very compatible](https://think-async.com/Asio/asio-1.38.2/doc/asio/using.html) same as boost without boost | 🟢 Yes                             |
| cpp-netlib        | 🟠 C++11             | 🔴 Very weak documentation (seems hardly accessible) | 🟠 Unknown, last updated 3 years ago                                                                           | 🟠 Seems vague or badly documented |
| POCO              | 🟠 C++17             | 🟠 Documentation is present, few examples            | 🟢 Compatible windows, last release end of September 2026                                                      | 🟢 Yes                             |

## Conclusion

We've decided to go with Boost Asio.
We could've gone with Asio "standalone" which is mostly a "copy" of Boost Asio without Boost but we think that Boost could help us in a couple of ways (threading library, endianness functions, math functions & more).

Not only this, but Boost Asio is widely recognized and used. The Boost.org website claims it's not only "Well tested by members of the C++ standards committee." but also "Production-ready". It's also been [audited](https://ostif.org/boost-audit-complete/) in the past to make sure it's robust.
