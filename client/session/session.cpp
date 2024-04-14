#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {
	this->packetHandler->recvMessage();
	this->packetHandler->recvBuffer();
	Sleep(-1);
}