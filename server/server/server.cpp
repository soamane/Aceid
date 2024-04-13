#include "server.h"

#include "../session/session.h"

Server::Server(boost::asio::io_context& context, short port)
    : connectionManager(ConnectionManager::GetInstance()), acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

void Server::start() {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(this->acceptor.get_executor());
    this->acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& errorCode) {
        if (!errorCode) {
            this->connectionManager->addConnection(socket);
            this->createSession(socket);
        }
        this->start();
    });
}

void Server::stop() {
    this->acceptor.close();
    this->connectionManager->closeAllConnections();
}

void Server::createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    std::shared_ptr<Session> session = std::make_shared<Session>(*socket);
    {
        session->run();
    }
}