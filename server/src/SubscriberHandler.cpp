#include "SubscriberHandler.hpp"
#include "ClientHandler.hpp"
#include <f-engine/ecs/components/DeadComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/observer/Subscriber.hpp>
#include <f-engine/ecs/systems/BoundarySystem.hpp>
#include <f-engine/ecs/systems/MoveSystem.hpp>
#include <functional>
#include <iostream>
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
    if (content.exists<fengine::ecs::DeadComponent>()) {
        auto component = content.retrieve<fengine::ecs::DeadComponent>();

        packets::EntityID payload = {component.entityId};
        packets::PacketHeader header = {
            .type = packets::PacketType::KillEntity,
            .size = sizeof(packets::EntityID),
            .playerUuid = "",
        };
        std::stringstream ss;

        packets::Serialization::serialize(ss, header, payload);
        std::cout << ss.str() << std::endl;
    }
}

void rtype::server::SubscriberHandler::processBoundary(shared_subscriber &subscriber) {
    fengine::ecs::observer::SubscriberContent content = subscriber->getContent();
    if (content.exists<fengine::ecs::PositionComponent>()) {
        auto component = content.retrieve<fengine::ecs::PositionComponent>();

        std::cout << "X: " << component.pos_x << ", Y: " << component.pos_y << std::endl;
    }
}
