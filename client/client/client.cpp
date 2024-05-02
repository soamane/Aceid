#include "client.h"

#include "session/session.h"

Client::Client(boost::asio::io_context& context)
	: m_resolver(context)
{ }

void Client::connect(std::string_view address, std::string_view port)
{
	boost::asio::ip::tcp::resolver::query query(address.data( ), port.data( ));
	boost::asio::ip::tcp::resolver::iterator iterator = m_resolver.resolve(query);

	boost::asio::ip::tcp::socket socket(m_resolver.get_executor( ));
	boost::system::error_code errorCode;

	boost::asio::connect(socket, iterator, errorCode);
	if (errorCode)
	{
		throw std::runtime_error("connection failed");
	}

	createSession(socket);
}

void Client::createSession(boost::asio::ip::tcp::socket& socket)
{
	std::make_unique<Session>(socket)->run( );
}
