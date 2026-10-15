#include "ClientHandler.hpp"
#include "SafeQueue.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>

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
