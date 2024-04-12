#pragma once
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio.hpp>

class Acceptor
{
public:
	Acceptor(boost::asio::io_service& ioService, short port);
	void Start();

private:
	void CreateSession();

private:
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;
};

#endif // !ACCEPTOR_H
