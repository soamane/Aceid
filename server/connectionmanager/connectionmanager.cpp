#include "connectionmanager.h"

void ConnectionManager::addConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    if (socket != nullptr) {
        this->connections.push_back(socket);
    }
}

void ConnectionManager::closeConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    if (socket != nullptr) {
        auto it = std::find(connections.begin(), connections.end(), socket);
        if (it != connections.end()) {
            connections.erase(it);
        }
    }
}

void ConnectionManager::closeAllConnections() {
    for (const auto& connection : connections) {
        if (connection->is_open()) {
            connection->close();
        }
    }
    connections.clear();
}

const std::size_t ConnectionManager::getConnectionsCount() const {
    return this->connections.size();
}
