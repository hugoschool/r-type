# Config of the game engine

## IConfig

This engine implements a pure virtual interface named IConfig. It contains :

```c++
    virtual void set(std::string, std::any) = 0;
    virtual std::optional<std::any> get(std::string) = 0;
    virtual bool hasChanged() = 0;
    virtual void resetChange() = 0;
```

The function set sets a config with a name and any value attached to it. It also notifies the config that is has changed. The get function gets the value from the given key. It is the programmer job to any_cast the return value of get.

## AConfig

The AConfig implements all of these functions.

## Exemple : ClientConfig

The ClientConfig class does nothing on its own and only has a constructor and destructor, and inherits from AConfig. It is however possible to implement any function related to configs in it, if needed.

