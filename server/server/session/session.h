#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

#include "../packethandler/packethandler.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(boost::asio::ip::tcp::socket& socket);

	void run();

private:
	boost::asio::ip::tcp::socket socket;
	std::shared_ptr<PacketHandler> packetHandler;
};

#endif // !SESSION_H
