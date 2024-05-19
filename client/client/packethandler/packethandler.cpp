#include "packethandler.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/protect/xorstring/xorstring.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)) {

}

const std::string PacketHandler::recvMessage() {
	const std::vector<char> msgBuffer = recvPacket().data;
	if (msgBuffer.empty()) {
		throw std::runtime_error(xorstr_("Failed to recv message packet"));
	}

	std::string receivedMessage = std::string(msgBuffer.begin(), msgBuffer.end());
	if (receivedMessage.empty()) {
		throw std::runtime_error(xorstr_("Failed to assemble the read packet"));
	}

	receivedMessage = DataEncryption::decryptCustomMethod(receivedMessage);

	return receivedMessage;
}

const std::vector<char> PacketHandler::recvBuffer() {
	std::vector<char> buffer = recvPacket().data;
	if (buffer.empty()) {
		throw std::runtime_error(xorstr_("Failed to recv buffer packet"));
	}

	buffer = DataEncryption::decryptBuffer(buffer);
	return buffer;
}

void PacketHandler::sendMessage(const std::string& message) {
	if (message.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument"));
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
		throw std::runtime_error(xorstr_("Failed to send packet size"));
	}

	const std::vector<char> buffer = std::vector<char>(packet.data.begin(), packet.data.end());
	m_socket.write_some(boost::asio::buffer(buffer), errorCode);
	if (errorCode) {
		throw std::runtime_error(xorstr_("Failed to send packet body"));
	}
}

const Packet PacketHandler::recvPacket() {
	Packet packet;
	{
		boost::system::error_code errorCode;
		boost::asio::read(m_socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error(xorstr_("Failed to recv packet size"));
		}

		packet.data.resize(packet.size);
		boost::asio::read(m_socket, boost::asio::buffer(packet.data), boost::asio::transfer_all(), errorCode);
		if (errorCode) {
			throw std::runtime_error(xorstr_("Failed to recv packet body"));
		}
	}

	return packet;
}
