#include "../connector/connector.h"

int main() {

	try {
		boost::asio::io_context context;
		Connector acceptor(context);
		{
			acceptor.Connect("127.0.0.1", "23");
		}
	}
	catch (const std::exception& exception) {
		// TODO: exception logic
	}

	return 0;
}