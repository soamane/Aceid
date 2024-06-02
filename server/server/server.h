#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

class Server {
public:
	Server(boost::asio::io_context& context, short port);

	void Start();
	void Stop();

private:
	void CreateSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif // !SERVER_H
