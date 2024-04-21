#include "session.h"
#include "../api/api.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
}

Session::~Session() {
	if (socket.is_open()) {
		// TODO: Create info log
		socket.close();
	}
}

void Session::run() {
	auto self(shared_from_this());

}