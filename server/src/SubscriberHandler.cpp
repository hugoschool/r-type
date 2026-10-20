#include "SubscriberHandler.hpp"
#include "ClientHandler.hpp"
#include <f-engine/ecs/components/DeadComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/components/VelocityComponent.hpp>
#include <f-engine/ecs/observer/Subscriber.hpp>
#include <f-engine/ecs/systems/BoundarySystem.hpp>
#include <f-engine/ecs/systems/MoveSystem.hpp>
#include <functional>
#include <memory>
#include <r-type/packets/Entity.hpp>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/Serialization.hpp>

rtype::server::SubscriberHandler::SubscriberHandler(rtype::server::ClientHandler &clientHandler)
    : _clientHandler(clientHandler) {
    shared_subscriber movement = std::make_shared<fengine::ecs::observer::Subscriber>();
    shared_subscriber boundary = std::make_shared<fengine::ecs::observer::Subscriber>();

    _subscribers = {
        {movement, std::bind(&SubscriberHandler::processMovement, this, movement)},
        {boundary, std::bind(&SubscriberHandler::processBoundary, this, boundary)},
    };
}

void rtype::server::SubscriberHandler::add(fengine::ecs::Registry &registry) {
    auto movementSystem = registry.systemManager.getSystem<fengine::ecs::MovementSystem>();
    auto boundarySystem = registry.systemManager.getSystem<fengine::ecs::BoundarySystem>();
    shared_subscriber movement = _subscribers[0].first;
    shared_subscriber boundary = _subscribers[1].first;

    movementSystem->addSubscriber(movement);
    boundarySystem->addSubscriber(boundary);
}

void rtype::server::SubscriberHandler::process() {
    for (auto &[subscriber, func] : _subscribers) {
        while (!subscriber->empty()) {
            func(subscriber);
        }
    }
}

void rtype::server::SubscriberHandler::processMovement(shared_subscriber &subscriber) {
    fengine::ecs::observer::SubscriberContent content = subscriber->getContent();

    handlePositionComponent(content);
    handleVelocityComponent(content);
    handleDeadComponent(content);
}

void rtype::server::SubscriberHandler::processBoundary(shared_subscriber &subscriber) {
    fengine::ecs::observer::SubscriberContent content = subscriber->getContent();

    handlePositionComponent(content);
    handleVelocityComponent(content);
    handleDeadComponent(content);
}

void rtype::server::SubscriberHandler::handleDeadComponent(fengine::ecs::observer::SubscriberContent &content) {
    fengine::ecs::DeadComponent dead;

    if (!content.existsAndRetrieve(dead))
        return;

    packets::EntityDied payload = {
        .entityId = dead.entityId,
    };

    packets::PacketHeader header = {
        .type = packets::PacketType::EntityDied,
        .size = sizeof(packets::EntityDied),
        .playerUuid = "",
    };
    header.setUuid(DUMMY_UUID);

    std::stringstream ss;
    auto ar = packets::Serialization::createSerializeArchive(ss);
    packets::Serialization::serialize(ar, header);
    packets::Serialization::serialize(ar, payload);

    _clientHandler.getMessageQueueHandler().sendToAllClients(ss.str());
}

void rtype::server::SubscriberHandler::handleVelocityComponent(fengine::ecs::observer::SubscriberContent &content) {
    fengine::ecs::VelocityComponent velocity;

    if (!content.existsAndRetrieve(velocity))
        return;

    packets::EntityVelocityChange payload = {
        .entityId = content.getEntityId(),
        .velocity = packets::Vector2D(velocity.vel_x, velocity.vel_y),
    };

    packets::PacketHeader header = {
        .type = packets::PacketType::EntityVelocityChange,
        .size = sizeof(packets::EntityVelocityChange),
        .playerUuid = "",
    };
    header.setUuid(DUMMY_UUID);

    std::stringstream ss;
    auto ar = packets::Serialization::createSerializeArchive(ss);
    packets::Serialization::serialize(ar, header);
    packets::Serialization::serialize(ar, payload);

    _clientHandler.getMessageQueueHandler().sendToAllClients(ss.str());
}

void rtype::server::SubscriberHandler::handlePositionComponent(fengine::ecs::observer::SubscriberContent &content) {
    fengine::ecs::PositionComponent position;

    if (!content.existsAndRetrieve(position))
        return;

    packets::EntityMove payload = {
        .entityId = content.getEntityId(),
        .coordinates = packets::Vector2D(position.pos_x, position.pos_y),
    };

    packets::PacketHeader header = {
        .type = packets::PacketType::EntityMove,
        .size = sizeof(packets::EntityMove),
        .playerUuid = "",
    };
    header.setUuid(DUMMY_UUID);

    std::stringstream ss;
    auto ar = packets::Serialization::createSerializeArchive(ss);
    packets::Serialization::serialize(ar, header);
    packets::Serialization::serialize(ar, payload);

    _clientHandler.getMessageQueueHandler().sendToAllClients(ss.str());
}
