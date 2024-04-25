#include "packethandler.h"

#include "../../secure/crypt/crypt.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}

const std::string PacketHandler::recvMessage() {
	std::vector<char> msgBuffer = recvPacket();
	if (msgBuffer.empty()) {
		throw std::runtime_error("failed to recv message packet");
	}

	std::string receivedMessage = std::string(msgBuffer.begin(), msgBuffer.end());
	receivedMessage = Crypt::decryptBase64(receivedMessage);

	return receivedMessage;
}

const std::vector<char> PacketHandler::recvBuffer() {
	std::vector<char> buffer = recvPacket();
	if (buffer.empty()) {
		throw std::runtime_error("failed to recv buffer packet");
	}

	return buffer;
}

void PacketHandler::sendMessage(std::string& message) {
	message = Crypt::encryptBase64(message);
	Packet packet;
	{
		packet.size = message.size();
		packet.data = std::vector<char>(message.begin(), message.end());
	}

	sendPacket(packet);
}

void PacketHandler::sendPacket(const Packet& packet) {
	boost::system::error_code errorCode;
	socket.write_some(boost::asio::buffer(&packet.size, sizeof(packet.size)), errorCode);
	if (errorCode) {
		throw std::runtime_error("failed to send packet size");
	}

	const std::vector<char> buffer = std::vector<char>(packet.data.begin(), packet.data.end());
	socket.write_some(boost::asio::buffer(buffer), errorCode);
	if (errorCode) {
		throw std::runtime_error("failed to send packet body");
	}
}

std::vector<char> PacketHandler::recvPacket() {
	Packet packet;
	{
		boost::system::error_code errorCode;
		boost::asio::read(socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("failed to recv packet size");
		}

		packet.data.resize(packet.size);
		boost::asio::read(socket, boost::asio::buffer(packet.data), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("failed to recv packet body");
		}
	}

	return packet.data;
}
