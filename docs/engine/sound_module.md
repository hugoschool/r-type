# Sound module of the game engine

This engine implements a pure virtual Sound module in order to play sounds and music while playing.

## ISoundModule

The ISoundModule interface implements three functions :

```c++
    virtual void playSound(std::string) = 0;
    virtual void playMusic(std::string) = 0;
    virtual void stopMusic(std::string) = 0;
```
Unlike the Display module, the Sound module does not add anything any component to the ECS. However, it could be used to play sounds at specific occurences, or following a timer in specific systems.

The musics played using playMusic are automatically looped, and calling the function stops the already playing music to let the new one play.
