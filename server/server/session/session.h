#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

#include "../packethandler/packethandler.h"

/*

	Class for the session of the connected socket

*/

class Session : public std::enable_shared_from_this<Session> {
public:

	// Constructor for socket initialization, packet handler, and event logging
	Session(boost::asio::ip::tcp::socket& socket);

	// Class destructor to close the socket upon Session object deletion
	~Session();

	// Main session handling function
	void run();

private:
	boost::asio::ip::tcp::socket m_socket; // current remote socket
	std::shared_ptr<PacketHandler> m_packetHandler;
};

#endif // !SESSION_H
