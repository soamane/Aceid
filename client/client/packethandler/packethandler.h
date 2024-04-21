#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <boost/asio.hpp>

struct Packet
{
	std::size_t size;
	std::vector<char> data;
};

class PacketHandler
{
public:
	PacketHandler(boost::asio::ip::tcp::socket& socket);

	void sendMessage(const std::string& message);

	const std::string recvMessage();
	const std::vector<char> recvBuffer();
	
private:
	void sendPacket(const Packet& packet);
	std::vector<char> recvPacket();

private:
	boost::asio::ip::tcp::socket socket;
};

#endif // !PACKET_HANDLER_H
