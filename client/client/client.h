#pragma once
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string_view>
#include <boost/asio.hpp>

class Client
{
public:
	Client(boost::asio::io_context& context);
	void Connect(std::string_view address, std::string_view port);

private:
	void CreateSession();

private:
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::resolver resolver;
};

#endif // !CONNECTOR_H
