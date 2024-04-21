#include "session.h"

#include "../api/api.h"

#include <fstream>

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {

}