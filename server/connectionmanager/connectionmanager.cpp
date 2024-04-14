#include "connectionmanager.h"

ConnectionManager* ConnectionManager::GetInstance()
{
    static ConnectionManager* Instance = new ConnectionManager();
    return Instance;
}

void ConnectionManager::addConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    if (socket != nullptr) {
        this->connections.push_back(socket);
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

ConnectionManager::ConnectionManager() {}
