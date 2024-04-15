#include "session.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
}

void Session::run() {
	auto self(shared_from_this());

	self->packetHandler->recvMessage([self](const std::string& message) {
		if (!message.empty()) {
			// TODO: error log
			return;
		}
		std::cout << message.c_str();
	});

}