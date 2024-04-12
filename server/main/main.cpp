#include "../server/server.h"

int main() {
	
	try {
		boost::asio::io_context context;
		Server server(context, 23);
		{
			server.Start();
			context.run();
		}
	} catch(const std::exception& exception) {
		// TODO: exception logic
	}

	return 0;
}