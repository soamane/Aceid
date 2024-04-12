#pragma once
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <boost/asio.hpp>

class Acceptor
{
public:
	Acceptor(boost::asio::io_context& context, short port);
	void Start();

private:
	void CreateSession(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket);

private:
	boost::asio::ip::tcp::acceptor acceptor;
};

#endif // !ACCEPTOR_H
