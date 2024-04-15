#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {
	if (!this->packetHandler->sendMessage("test_message_#1")) {
		// TODO: error log
		return;
	}

	Sleep(-1);
}