#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
}

void Session::run() {
	auto self(shared_from_this());
	this->packetHandler->sendMessage("hello, client! take my message: ", [self](bool status) {
		if (status) {
			self->packetHandler->sendBuffer({'1', '3', 'g', 'c'}, [self](bool status) {

			});
		}
	});
}