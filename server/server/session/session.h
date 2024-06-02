#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

#include "../packethandler/packethandler.h"

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(boost::asio::ip::tcp::socket& socket);
	~Session();

	void Open();

private:
	boost::asio::ip::tcp::socket m_socket; 
	std::shared_ptr<PacketHandler> m_packetHandler;
};

#endif // !SESSION_H
