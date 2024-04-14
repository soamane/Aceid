#include "session.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
}

void Session::run() {

	std::string str = "hello, client!";
	Packet packet;
	{
		packet.size = str.size();
		packet.data = std::vector<char>(str.begin(), str.end());
	}

	this->packetHandler->sendPacket(packet);
}