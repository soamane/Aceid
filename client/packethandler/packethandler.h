#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <boost/asio.hpp>

class PacketHandler
{
public:
	PacketHandler(boost::asio::ip::tcp::socket& socket);

	void recvMessage();
	void recvBuffer();

private:
	boost::asio::ip::tcp::socket socket;
};

#endif // !PACKET_HANDLER_H
