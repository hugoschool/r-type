# Scenes of the game engine

This game engine implements a pure virtual interface for Scenes.

## Scene Objects

Scene Objects are what the Scenes hold. Already implemented are Buttons, the Background and Texts.

Buttons is a class that has a function isInBounds, indicating if our mouse is on the button, thus making it able to click on them.

## IScene

The IScene has pure virtual functions implemented by the AScene, that are :

```cpp
    virtual std::map<int, Button> &getButtons() = 0;
    virtual Background &getBackground() = 0;
    virtual std::vector<Text> &getTexts() = 0;

    virtual void updateButtons(std::pair<float, float> &) = 0;

    virtual bool empty() = 0;
    virtual int pollEvents() = 0;
```

Three getters for the Scene Objects, updateButtons that checks if the coordinates given are on a button and adds the associated number of the button to a queue. Empty that checks if the button queue clicks is empty, and poll event that retrieves the first number in the queue and lets the client interpret it however he wants.

