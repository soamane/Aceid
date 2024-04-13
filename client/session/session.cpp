#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), communicationHandler(std::make_unique<CommunicationHandler>(this->socket)) { }

void Session::run() {

}