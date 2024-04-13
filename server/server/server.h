#pragma once
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio.hpp>

#include "../connectionmanager/connectionmanager.h"

class Server
{
public:
	Server(boost::asio::io_context& context, short port);

	void start();
	void stop();

private:
	void createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

private:
	boost::asio::ip::tcp::acceptor acceptor;
	std::unique_ptr<ConnectionManager> connectionManager;
};

#endif // !ACCEPTOR_H
