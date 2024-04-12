#include "connector.h"

Connector::Connector(boost::asio::io_service& ioService)
	: socket(ioService), resolver(ioService) {
}

void Connector::Connect(std::string_view address, std::string_view port) {
	boost::asio::ip::tcp::resolver::query query(address.data(), port.data());
	boost::asio::ip::tcp::resolver::iterator iterator = this->resolver.resolve(query);

	boost::system::error_code errorCode{ };
	boost::asio::connect(this->socket, iterator, errorCode);
	if (errorCode) {
		// TODO: exception logic
	}

	this->CreateSession();
}

void Connector::CreateSession() {
}
