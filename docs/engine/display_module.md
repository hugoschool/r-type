# Display module of the game engine

This game engine also comes with a pure virtual implementation of a graphic display.

## ECS : Drawable Component and Display System

For the graphic only, a DrawableComponent is implemented, holding :

```c++
std::optional<std::string> texture_filepath;
std::shared_ptr<IShape> shape;
std::optional<std::tuple<int, int, int>> color;
```

It is used by a DisplaySystem, which draws each entity based on a position component and a drawable component.

## Shapes

The IShape is a pure virtual interface with three important methods :

```c++
virtual void setPosition(PositionComponent &) = 0;
virtual void setColor(std::tuple<int, int, int> &) = 0;
virtual void setTexture(std::any) = 0;
```
Those are the three methods used to customize the shape.

For example, it could be an SFMLRectangle, inheriting from IShape. The shape you are creating depend on the graphical library you want to implement.

## Window

In order to draw those IShapes, there is an IWindow interface that implements a drawShape method. When adding a new library, you need to create another Window class inheriting from the IWindow interface, and then implement the drawShape function.

## Events

There is an IEvent pure virtual interface.

You can implement any kind of Event as long as it is handled by the library you are using.

The Events have custom types, keys, mouse buttons enums in order to be virtual.

## Display Module

Last but not least, there is the IDisplayModule interface, implementing three pure virtual methods :

```c++
virtual void clear() = 0;
virtual void drawEntity(PositionComponent &, DrawableComponent &) = 0;
virtual std::optional<std::unique_ptr<IEvent>> pollEvent() = 0;
```

Two of these methods, clear and pollEvent, are used in the main display loop of the game.

The drawEntity method is used by the DisplaySystem, which holds a copy of the displayModule.
