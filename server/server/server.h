#pragma once
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio.hpp>

/*

	Fundamental class for initializing the server

*/
class Server {
public:

	// Initializes the context and server port
	Server(boost::asio::io_context& context, short port);

	// Server startup
	void start();

	// Server shutdown
	void stop();

private:

	// Creating a unique session for the connected socket
	void createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif // !ACCEPTOR_H
