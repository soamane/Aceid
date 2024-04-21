#include "packethandler.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}

const std::string PacketHandler::recvMessage() {
	std::vector<char> msgBuffer = this->recvPacket();
	if (msgBuffer.empty()) {
		throw std::runtime_error("failed to recv message packet");
	}

	return std::string(msgBuffer.begin(), msgBuffer.end());
}

const std::vector<char> PacketHandler::recvBuffer() {
	std::vector<char> buffer = this->recvPacket();
	if (buffer.empty()) {
		throw std::runtime_error("failed to recv buffer packet");
	}

	return buffer;
}

void PacketHandler::sendMessage(const std::string& message) {
	Packet packet;
	{
		packet.size = message.size();
		packet.data = std::vector<char>(message.begin(), message.end());
	}

	this->sendPacket(packet);
}

void PacketHandler::sendPacket(const Packet& packet) {
	boost::system::error_code errorCode;
	this->socket.write_some(boost::asio::buffer(&packet.size, sizeof(packet.size)), errorCode);
	if (errorCode) {
		throw std::runtime_error("failed to send packet size");
	}

	const std::vector<char> buffer = std::vector<char>(packet.data.begin(), packet.data.end());
	this->socket.write_some(boost::asio::buffer(buffer), errorCode);
	if (errorCode) {
		throw std::runtime_error("failed to send packet body");
	}
}

std::vector<char> PacketHandler::recvPacket() {
	Packet packet;
	{
		boost::system::error_code errorCode;
		boost::asio::read(this->socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("failed to recv packet size");
		}

		packet.data.resize(packet.size);
		boost::asio::read(this->socket, boost::asio::buffer(packet.data), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("failed to recv packet body");
		}
	}

	return packet.data;
}
