#include "client.h"

#include "session/session.h"

#include "../general/protect/xorstring/xorstring.h"

Client::Client(boost::asio::io_context& context) : m_resolver(context), m_socket(context) { }

void Client::Connect(std::string_view address, std::string_view port) {
	boost::asio::ip::tcp::resolver::query query(address.data(), port.data());
	boost::asio::ip::tcp::resolver::iterator iterator = m_resolver.resolve(query);

	boost::system::error_code errorCode;
	boost::asio::connect(m_socket, iterator, errorCode);
	if (errorCode) {
		throw std::runtime_error(xorstr_("Failed to connect to the server"));
	}

	Session session(m_socket);
	session.Open();
}