#include "ClientHandler.hpp"
#include "Exception.hpp"
#include "SafeQueue.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>

rtype::server::ClientMessageQueueHandler::ClientMessageQueueHandler() : _map(), _mutex() {
}

void rtype::server::ClientMessageQueueHandler::addMessage(std::string uuid, std::string content) {
    std::unique_lock<std::mutex> lock(_mutex);

    if (!_map.contains(uuid)) {
        _map.insert({uuid, SafeMessageQueue{}});
    }
    _map[uuid].push(content);
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

rtype::server::ClientHandler::ClientHandler()
    : _maxStatusSecondsMap({
          {ClientStatus::Disconnected, std::chrono::seconds(MAX_DISCONNECTED_SECONDS)},
          {ClientStatus::Connecting, std::chrono::seconds(MAX_CONNECTING_SECONDS)},
          {ClientStatus::Connected, std::chrono::seconds(MAX_CONNECTED_SECONDS)},
      }),
      _clients() {
}

std::string rtype::server::ClientHandler::generateUuid() {
    return boost::lexical_cast<std::string>(boost::uuids::to_string(boost::uuids::random_generator()()));
}

void rtype::server::ClientHandler::addClient(std::string &uuid) {
    ClientData data = {
        .status = ClientStatus::Disconnected,
        .timer = std::chrono::steady_clock::now(),
    };

    _clients.insert({uuid, data});
}

void rtype::server::ClientHandler::setStatus(std::string &uuid, ClientStatus newStatus) {
    ClientData &data = _clients.at(uuid);

    data.status = newStatus;
    if (data.status == ClientStatus::Connected) {
        data.timer = std::chrono::steady_clock::now();
    }
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
