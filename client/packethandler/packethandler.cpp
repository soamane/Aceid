#include "packethandler.h"

#include <iostream>

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}

void PacketHandler::recvMessage() {
	std::size_t size;
	boost::asio::read(this->socket, boost::asio::buffer(&size, sizeof(size)), boost::asio::transfer_all());

	std::vector<char> buffer(size);
	boost::asio::read(this->socket, boost::asio::buffer(buffer), boost::asio::transfer_all());

	std::cout << std::string(buffer.begin(), buffer.end());
}

void PacketHandler::recvBuffer() {
	std::size_t size;
	boost::asio::read(this->socket, boost::asio::buffer(&size, sizeof(size)), boost::asio::transfer_all());

	std::vector<char> buffer(size);
	boost::asio::read(this->socket, boost::asio::buffer(buffer), boost::asio::transfer_all());

	for (auto i : buffer) {
		std::cout << i << ' ';
	}
}
