#include "client.h"

#include "session/session.h"

Client::Client(boost::asio::io_context& context)
	: resolver(context) {
}

void Client::connect(std::string_view address, std::string_view port) {
	boost::asio::ip::tcp::resolver::query query(address.data(), port.data());
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

	boost::asio::ip::tcp::socket socket(resolver.get_executor());
	boost::system::error_code errorCode;

	boost::asio::connect(socket, iterator, errorCode);
	if (errorCode) {
		throw std::runtime_error("connection failed");
	}

	createSession(socket);
}

void Client::createSession(boost::asio::ip::tcp::socket& socket) {
	std::make_unique<Session>(socket)->run();
}
