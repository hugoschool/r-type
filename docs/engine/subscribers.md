# Subscribers

A detailed explanation of the Observer pattern used in the engine systems.

## Explanation

Inside the R-Type, the server must know whenever something has been updated by a System.

To do so, we use an [Observer pattern](https://refactoring.guru/design-patterns/observer).

Essentially, the server (or whichever part wishes to connect to a system, referred to as a "consumer" in this document) creates `Subscribers` which then subscribe to a system to retrieve all the needed information.

## Usage

### Engine / System side

Concerning a system, you must create a `SubscriberContent` for each entity you wish to send to your server.

Once created, you must add each component to send to the client then use the `updateSubscribers` function to update all of your subscribers with the `SubscriberContent` variable.

```cpp
std::size_t entityId = 12;
SubscriberContent content(entityId);
content.add<PositionComponent>(pos);
content.add<VelocityComponent>(vel);
updateSubscribers(content);
```

### Server / consumer side

On the consumer, you first have to register your subscriber to the system:

```cpp
auto system = registry.systemManager.getSystem<fengine::ecs::MovementSystem>();
fengine::ecs::observer::Subscriber subscriber();

system->addSubscriber(subscriber);
```

Now you can use the `getContent` function to retrieve the latest content from the system. After this you can use the `existsAndRetrieve` function to get a component only if it exists in the `SubscriberContent`:

```cpp
fengine::ecs::observer::SubscriberContent content = subscriber.getContent();
fengine::ecs::PositionComponent position;

if (!content.existsAndRetrieve(position))
    return;
std::cout << "X: " << content.pos_x << std::endl;
std::cout << "Y: " << content.pos_y << std::endl;
```
