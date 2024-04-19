#include "packethandler.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}

const std::string PacketHandler::recvMessage() {
	std::vector<char> msgBuffer = this->recvPacket();
	if (msgBuffer.empty()) {
		// TODO: error log
		return std::string();
	}

	return std::string(msgBuffer.begin(), msgBuffer.end());
}

const std::vector<char> PacketHandler::recvBuffer() {
	std::vector<char> buffer = this->recvPacket();
	if (buffer.empty()) {
		// TODO: error log
		return std::vector<char>();
	}

	return buffer;
}

bool PacketHandler::sendMessage(const std::string& message) {
	Packet packet;
	{
		packet.size = message.size();
		packet.data = std::vector<char>(message.begin(), message.end());
	}

	if (!this->sendPacket(packet)) {
		// TODO: error log
		return false;
	}

	return true;
}

bool PacketHandler::sendPacket(const Packet& packet) {
	boost::system::error_code errorCode;
	this->socket.write_some(boost::asio::buffer(&packet.size, sizeof(packet.size)), errorCode);
	if (errorCode) {
		// TODO: error log
		return false;
	}

	const std::vector<char> buffer = std::vector<char>(packet.data.begin(), packet.data.end());
	this->socket.write_some(boost::asio::buffer(buffer), errorCode);
	if (errorCode) {
		// TODO: error log
		return false;
	}

	return true;
}

std::vector<char> PacketHandler::recvPacket() {
	Packet packet;
	{
		boost::system::error_code errorCode;
		boost::asio::read(this->socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			// TODO: error log
			return std::vector<char>();
		}

		packet.data.resize(packet.size);
		boost::asio::read(this->socket, boost::asio::buffer(packet.data), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			// TODO: error log
			return std::vector<char>();
		}
	}

	return packet.data;
}
