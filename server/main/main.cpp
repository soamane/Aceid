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
		MessageBoxA(GetForegroundWindow(), exception.what(), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}