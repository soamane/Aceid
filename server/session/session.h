#pragma once
#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <boost/asio.hpp>

#include "../communicationhandler/communicationhandler.h"

class Session
{
public:
	Session(boost::asio::ip::tcp::socket& socket);

	void run();

private:
	boost::asio::ip::tcp::socket socket;
	std::shared_ptr<CommunicationHandler> communicationHandler;
};

#endif // !SESSION_H
