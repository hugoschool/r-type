#include "ClientHandler.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <chrono>

rtype::server::ClientHandler::ClientHandler() : _clients() {
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
