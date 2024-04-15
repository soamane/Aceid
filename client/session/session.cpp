#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {
	if (!this->packetHandler->sendMessage("hello, server!")) {
		// TODO: error log
		return;
	}

	Sleep(-1);
}