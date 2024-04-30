#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <boost/asio.hpp>

struct Packet
{
	std::size_t size;
	std::vector<char> data;
};

enum EServerResponse 
{
	eSR_SUCCESS = 0,
	eSR_ERROR
};

class PacketHandler
{
public:
	PacketHandler(boost::asio::ip::tcp::socket& socket);

	void sendMessage(const std::string& message);

	const std::string recvMessage();
	const std::vector<char> recvBuffer();
	const EServerResponse recvServerResponse();
	
private:
	void sendPacket(const Packet& packet);
	std::vector<char> recvPacket();

private:
	boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
