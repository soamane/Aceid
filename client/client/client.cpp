#include "client.h"

#include <iostream>

Client::Client(boost::asio::io_context& context)
	: socket(context), resolver(context) {
}

void Client::connect(std::string_view address, std::string_view port) {
	boost::asio::ip::tcp::resolver::query query(address.data(), port.data());
	boost::asio::ip::tcp::resolver::iterator iterator = this->resolver.resolve(query);

	boost::system::error_code errorCode{ };
	boost::asio::connect(this->socket, iterator, errorCode);
	if (errorCode) {
		// TODO: exception logic
	}

	this->createSession();
}

void Client::createSession() {
	std::cout << "connected";
	Sleep(-1);
}
