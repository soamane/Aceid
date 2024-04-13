#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

class Session
{
public:
	Session(boost::asio::ip::tcp::socket& socket);

	void run();

private:
	boost::asio::ip::tcp::socket socket;
};

#endif // !SESSION_H
