#include "server.h"

Server::Server(boost::asio::io_context& context, short port)
	: acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

void Server::start() {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(this->acceptor.get_executor());
    this->acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& errorCode) {
        if (!errorCode) {
            this->createSession(socket);
        }
        this->start();
    });
}

void Server::createSession(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket) {

}
