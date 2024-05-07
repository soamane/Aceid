#include "packethandler.h"

#include "../../general/protect/dataencryption/dataencryption.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)) {

}

const std::string PacketHandler::recvMessage() {
	std::vector<char> msgBuffer = recvPacket();
	if (msgBuffer.empty()) {
		throw std::runtime_error("Failed to recv message packet");
	}

	std::string receivedMessage = std::string(msgBuffer.begin(), msgBuffer.end());
	if (receivedMessage.empty()) {
		throw std::runtime_error("Failed to assemble the read packet");
	}

	receivedMessage = DataEncryption::decryptCustomMethod(receivedMessage);

	return receivedMessage;
}

const std::vector<char> PacketHandler::recvBuffer() {
	std::vector<char> buffer = recvPacket();
	if (buffer.empty()) {
		throw std::runtime_error("Failed to recv buffer packet");
	}

	buffer = DataEncryption::decryptBuffer(buffer);
	return buffer;
}

void PacketHandler::sendMessage(const std::string& message) {
	if (message.empty()) {
		throw std::runtime_error("Function call error: empty argument");
	}

	const std::string encryptedMessage = DataEncryption::encryptCustomMethod(message);
	Packet packet;
	{
		packet.size = encryptedMessage.size();
		packet.data = std::vector<char>(encryptedMessage.begin(), encryptedMessage.end());
	}

	sendPacket(packet);
}

void PacketHandler::sendPacket(const Packet& packet) {
	boost::system::error_code errorCode;
	m_socket.write_some(boost::asio::buffer(&packet.size, sizeof(packet.size)), errorCode);
	if (errorCode) {
		throw std::runtime_error("Failed to send packet size");
	}

	const std::vector<char> buffer = std::vector<char>(packet.data.begin(), packet.data.end());
	m_socket.write_some(boost::asio::buffer(buffer), errorCode);
	if (errorCode) {
		throw std::runtime_error("Failed to send packet body");
	}
}

std::vector<char> PacketHandler::recvPacket() {
	Packet packet;
	{
		boost::system::error_code errorCode;
		boost::asio::read(m_socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("Failed to recv packet size");
		}

		packet.data.resize(packet.size);
		boost::asio::read(m_socket, boost::asio::buffer(packet.data), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error("Failed to recv packet body");
		}
	}

	return packet.data;
}
