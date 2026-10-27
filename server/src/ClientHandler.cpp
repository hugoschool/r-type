#include "ClientHandler.hpp"
#include "Exception.hpp"
#include "SafeQueue.hpp"
#include "r-type/components/AttackComponent.hpp"
#include "r-type/components/DefenseComponent.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <f-engine/ecs/components/ControllableComponent.hpp>
#include <f-engine/ecs/components/PositionComponent.hpp>
#include <f-engine/ecs/components/VelocityComponent.hpp>
#include <iostream>
#include <r-type/packets/Packet.hpp>
#include <r-type/packets/PacketBuilder.hpp>
#include <r-type/packets/Payload.hpp>
#include <r-type/packets/Serialization.hpp>

rtype::server::ClientMessageQueueHandler::ClientMessageQueueHandler() : _map(), _disconnectedClients(), _mutex() {
}

void rtype::server::ClientMessageQueueHandler::createClient(std::string uuid) {
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_map.contains(uuid)) {
        _map.insert({uuid, SafeMessageQueue{}});
    }
}

void rtype::server::ClientMessageQueueHandler::addMessage(std::string uuid, std::string content) {
    if (uuid == "") {
        return;
    }
    if (_disconnectedClients.contains(uuid)) {
        return;
    }

    std::cout << "Adding a message to " << uuid << std::endl;

    createClient(uuid);

    {
        std::unique_lock<std::mutex> lock(_mutex);
        _map[uuid].push(content);
    }
}

bool rtype::server::ClientMessageQueueHandler::isClientEmpty(std::string uuid) {
    std::unique_lock<std::mutex> lock(_mutex);

    try {
        auto &queue = _map.at(uuid);
        return queue.empty();
    } catch (std::exception &) { throw Exception("No queue for client " + uuid); }
}

std::string rtype::server::ClientMessageQueueHandler::popMessage(std::string uuid) {
    std::unique_lock<std::mutex> lock(_mutex);

    try {
        auto &queue = _map.at(uuid);
        return queue.pop();
    } catch (std::exception &) { throw Exception("No queue for client " + uuid); }
}

std::map<std::string, rtype::server::SafeMessageQueue> &rtype::server::ClientMessageQueueHandler::getMap() {
    return _map;
}

void rtype::server::ClientMessageQueueHandler::sendToAllClients(std::string content) {
    for (auto &[uuid, _] : _map) {
        addMessage(uuid, content);
    }
}

void rtype::server::ClientMessageQueueHandler::disconnectClient(std::string uuid) {
    _disconnectedClients.insert(uuid);

    while (!isClientEmpty(uuid)) {
        popMessage(uuid);
    }
}

rtype::server::ClientHandler::ClientHandler(fengine::ecs::Registry &registry, ServerPacketBuilder &packetBuilder)
    : _maxStatusSecondsMap({
          {ClientStatus::Disconnected, std::chrono::seconds(MAX_DISCONNECTED_SECONDS)},
          {ClientStatus::Connecting, std::chrono::seconds(MAX_CONNECTING_SECONDS)},
          {ClientStatus::Connected, std::chrono::seconds(MAX_CONNECTED_SECONDS)},
      }),
      _clients(), _uuids(), _registry(registry), _packetBuilder(packetBuilder) {
    for (std::size_t i = 0; i < MAX_PLAYERS; i++) {
        _uuids.push(generateUuid());
    }
}

std::string rtype::server::ClientHandler::generateUuid() {
    return boost::lexical_cast<std::string>(boost::uuids::to_string(boost::uuids::random_generator()()));
}

std::unordered_map<std::string, rtype::server::ClientData> &rtype::server::ClientHandler::getClients() {
    return _clients;
}

std::string rtype::server::ClientHandler::newClient() {
    std::string uuid = _uuids.front();
    _uuids.pop();

    _messageQueueHandler.createClient(uuid);

    fengine::ecs::Entity entity = _registry.entityManager.spawnEntity();

    ClientData data = {
        .status = ClientStatus::Disconnected,
        .entityId = entity.getId(),
        .timer = std::chrono::steady_clock::now(),
    };

    _clients.insert({uuid, data});

    fengine::ecs::PositionComponent pos = {50, 50, false};
    fengine::ecs::VelocityComponent vel = {0, 0, true};
    fengine::ecs::AttackComponent att = {{.team = fengine::ecs::team::ALLIES}, 10};
    fengine::ecs::DefenseComponent def = {100, 0};

    _registry.componentManager.addComponent(entity, std::move(pos));
    _registry.componentManager.addComponent(entity, std::move(vel));
    _registry.componentManager.addComponent(entity, std::move(att));
    _registry.componentManager.addComponent(entity, std::move(def));

    packets::LoginAnswer payload;
    payload.setUuid(uuid);

    std::string content = _packetBuilder.create(packets::PacketType::LoginAnswer).addHeader().addPayload(payload).get();
    _messageQueueHandler.addMessage(uuid, content);

    return uuid;
}

void rtype::server::ClientHandler::setStatus(std::string &uuid, ClientStatus newStatus) {
    ClientData &data = _clients.at(uuid);

    data.status = newStatus;
    if (data.status == ClientStatus::Connected) {
        data.timer = std::chrono::steady_clock::now();
    }
}

bool rtype::server::ClientHandler::clientExists(std::string &uuid) {
    return _clients.contains(uuid);
}

std::size_t rtype::server::ClientHandler::getClientsAmount() {
    return _clients.size();
}

void rtype::server::ClientHandler::setAlive(std::string &uuid) {
    ClientData &data = _clients.at(uuid);
    data.status = ClientStatus::Connected;
    data.timer = std::chrono::steady_clock::now();
}

void rtype::server::ClientHandler::verifyStatus() {
    for (auto &[uuid, data] : _clients) {
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = currentTime - data.timer;

        if (data.status == ClientStatus::Removed || duration < _maxStatusSecondsMap[data.status]) {
            continue;
        }

        std::cout << "Previous status: " << std::to_string(static_cast<int>(data.status)) << std::endl;

        switch (data.status) {
            case ClientStatus::Disconnected: {
                const std::string uuidCopy = uuid;
                data.status = ClientStatus::Removed;
                _messageQueueHandler.disconnectClient(uuidCopy);
                std::cout << "Client " << uuidCopy << " erased" << std::endl;
                break;
            }
            case ClientStatus::Connecting:
                data.status = ClientStatus::Disconnected;
                break;
            case ClientStatus::Connected:
                data.status = ClientStatus::Connecting;
                break;
            case ClientStatus::Removed:
                break;
        }

        std::cout << "New status: " << std::to_string(static_cast<int>(data.status)) << std::endl;

        if (data.status == ClientStatus::Removed)
            continue;

        data.timer = currentTime;
    }
}

rtype::server::ClientMessageQueueHandler &rtype::server::ClientHandler::getMessageQueueHandler() {
    return _messageQueueHandler;
}
