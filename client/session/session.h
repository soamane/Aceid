#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../packethandler/packethandler.h"

class Session
{
public:
	Session(boost::asio::ip::tcp::socket& socket);

	void run();

private:
	boost::asio::ip::tcp::socket socket;
	std::unique_ptr<PacketHandler> packetHandler;
};

#endif // !SESSION_H
