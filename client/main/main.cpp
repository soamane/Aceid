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
		// TODO: exception logic
	}

	return 0;
}