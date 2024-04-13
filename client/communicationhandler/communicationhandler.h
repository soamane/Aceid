#pragma once
#ifndef COMMUNICATION_HANDLER_H
#define COMMUNICATION_HANDLER_H

#include <boost/asio.hpp>

class CommunicationHandler
{
public:
	CommunicationHandler(boost::asio::ip::tcp::socket& socket);

private:
	boost::asio::ip::tcp::socket socket;
};

#endif // !COMMUNICATION_HANDLER_H
