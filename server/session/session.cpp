#include "session.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
}

void Session::run() {
	auto self(shared_from_this());

	self->packetHandler->sendMessage("test_packet_1", [self](bool status) {
		if (status) {
			self->packetHandler->recvMessage([self](const std::string& callback) {
				std::cout << callback.c_str();
			});
		}
	});

}