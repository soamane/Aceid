#include "../server/server.h"

#include <iostream>

int main() {

	try {
		boost::asio::io_context context;
		Server server(context, 23);
		{
			server.start();
			context.run();
		}
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what();
	}

	return 0;
}