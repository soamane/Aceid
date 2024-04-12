#include "acceptor.h"

Acceptor::Acceptor(boost::asio::io_service& ioService, short port)
	: socket(ioService), acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) { }

void Acceptor::Start() {
	this->acceptor.async_accept(this->socket, [this](boost::system::error_code errorCode) {
		if (errorCode) {
			// TODO: exception logic
		}
		CreateSession();
		Start();
	});
}

void Acceptor::CreateSession() {

}
