#include "PriorityHandler.hpp"
#include "r-type/packets/Packet.hpp"
#include <cstddef>
#include <exception>
#include <map>
#include <mutex>
#include <string>
#include <utility>

rtype::server::PriorityHandler::PriorityHandler() : _playersHighPriorityMsg() {
}

rtype::server::PriorityHandler::~PriorityHandler() {
}

void rtype::server::PriorityHandler::addClient(std::string uuid) {
    if (!_playersHighPriorityMsg.contains(uuid)) {
        std::unique_lock lock(_mutex);
        _playersHighPriorityMsg.insert({uuid, std::map<size_t, std::pair<packets::PacketType, payloadType>>()});
    }
}

void rtype::server::PriorityHandler::addMessage(std::string uuid, size_t id, rtype::packets::PacketType type, payloadType message) {

    try {
        std::unique_lock lock(_mutex);
        auto &queue = _playersHighPriorityMsg.at(uuid);
        queue.insert({id, {type, message}});
    } catch (std::exception &) {
    }
}

void rtype::server::PriorityHandler::addAll(size_t id, rtype::packets::PacketType type, payloadType message) {
    std::unique_lock lock(_mutex);
    for (auto &elem : _playersHighPriorityMsg) {
        auto &queue = elem.second;
        queue.insert({id, {type, message}});
    }
}

void rtype::server::PriorityHandler::removeClient(std::string uuid) {
    try {
        std::unique_lock lock(_mutex);
        auto &queue = _playersHighPriorityMsg.at(uuid);
        queue.clear();
        _playersHighPriorityMsg.erase(uuid);
    } catch (std::exception &) {
    }
}

std::map<size_t, std::pair<rtype::packets::PacketType, rtype::server::payloadType>> rtype::server::PriorityHandler::getMessage(std::string uuid) {
    auto &queue = _playersHighPriorityMsg.at(uuid);
    return queue;
}

void rtype::server::PriorityHandler::removeMessage(std::string uuid, size_t id) {
    try {
        std::unique_lock lock(_mutex);
        auto &map = _playersHighPriorityMsg.at(uuid);
        map.erase(id);
    } catch (std::exception &) {
    }
}
