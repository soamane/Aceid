#include "../client/client.h"

int main() {

	try {
		boost::asio::io_context context;
		Client client(context);
		{
			client.connect("127.0.0.1", "23");
		}
	}
	catch (const std::exception& exception) {
		MessageBoxA(GetForegroundWindow(), exception.what(), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}