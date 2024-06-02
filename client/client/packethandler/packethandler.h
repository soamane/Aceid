#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <boost/asio.hpp>

enum EServerResponse {
	FAILED_AUTH,
	SUCCESS_AUTH
};

struct Packet {
	std::size_t size;
	std::vector<char> data;
};

class PacketHandler {
public:
	PacketHandler(boost::asio::ip::tcp::socket& socket);

	void SendClientMessage(const std::string& message);

	const EServerResponse ReceiveServerResponse();
	const std::string ReceiveMessage();
	const std::vector<char> ReceiveDataBuffer();

private:
	void SendPacket(const Packet& packet);
	const Packet ReceivePacket();

private:
	boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
