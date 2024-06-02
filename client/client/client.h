#pragma once
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <string_view>
#include <boost/asio.hpp>

class Client {
public:
	Client(boost::asio::io_context& context);
	void Connect(std::string_view address, std::string_view port);

private:
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
};

#endif // !CONNECTOR_H
