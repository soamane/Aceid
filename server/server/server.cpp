#include "server.h"

#include "session/session.h"
#include "../general/logsystem/logmanager/logmanager.h"

#include <iostream>

Server::Server(boost::asio::io_context& context, short port)
    : m_acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

void Server::Start() {
    auto newConnection = std::make_shared<boost::asio::ip::tcp::socket>(m_acceptor.get_executor());
    if (newConnection == nullptr) {
        CREATE_SERVER_LOG("Failed init new connection");
    }

    m_acceptor.async_accept(*newConnection, [this, newConnection](boost::system::error_code errorCode) {
        if (!errorCode) {
            CreateSession(newConnection);
            std::cout << "[Server]: New connection detected" << std::endl;
        }
        Start();
    });
}

void Server::Stop() {
    if (m_acceptor.is_open()) {
        m_acceptor.close();
        CREATE_SERVER_LOG("Server closed");
    }
}

void Server::CreateSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    auto newSession = std::make_shared<Session>(*socket);
    if (newSession == nullptr) {
        CREATE_SERVER_LOG("Failed init new session");
        return;
    }

    newSession->Run();
}