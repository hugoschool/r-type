# Asset Manager of the game engine

This engine implements a pure virtual Asset Manager.

## IAssetManager

The IAssetManager class does not implement any function. While it could be possible to implement a normal AssetManager class, using std::type_index and std::any to store data, the short amount of time we had for this project limited our possibilities and time of thinking.

Since it does not declare any functions, the classes inheriting from the IAssetManager must implement their own way of storing data.

## SFMLAssetManager

As an exemple, the SFMLAssetManager looks like this :

```cpp
    void loadType(std::string filepath);
    std::optional<sf::Texture> &getType(std::string filepath);
    bool textureType(std::string filepath);
    std::map<std::string, std::optional<sf::Type>> _types;
```
Type is here an object that represents an SFML Object. It could be a sf::Texture, a sf::Music, etc...

For each of these function and the map, there is a type attached. If you want to load Textures and Musics, you would have to do :

```cpp
    void loadTexture(std::string filepath);
    void loadMusic(std::string filepath);
```

## Modules and Asset Manager

The modules (Display or Sound) hold an AssetManager according to the library they are implementing. For example, the SFMLDisplayModule holds a SFMLAssetManager.
