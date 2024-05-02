#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>

#include "../packethandler/packethandler.h"

class Session
{
public:
	Session(boost::asio::ip::tcp::socket& socket);
	~Session( );

	void run( );

private:
	boost::asio::ip::tcp::socket m_socket;
	std::unique_ptr<PacketHandler> m_packetHandler;
};

#endif // !SESSION_H
