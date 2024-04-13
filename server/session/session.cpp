#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), communicationHandler(std::make_shared<CommunicationHandler>(this->socket)) {
}

void Session::run() {
	this->communicationHandler->SendPacket("hi!");
}