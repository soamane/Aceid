#include "server.h"

#include "session/session.h"
#include "../general/logsystem/logmanager/logmanager.h"

Server::Server(boost::asio::io_context& context, short port) : m_acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

void Server::Start() {
    m_acceptor.async_accept([this](boost::system::error_code errorCode, boost::asio::ip::tcp::socket socket) {
        if (!errorCode) {
            CreateSession(socket);
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

void Server::CreateSession(boost::asio::ip::tcp::socket& socket) {
    std::shared_ptr<Session> session = std::make_shared<Session>(socket);
    if (session == nullptr) {
        CREATE_SERVER_LOG("Failed init new session");
        return;
    }

    session->Open();
}
