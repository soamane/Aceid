#include "../acceptor/acceptor.h"

int main() {
	
	try {
		boost::asio::io_context context;
		Acceptor acceptor(context, 23);
		{
			acceptor.Start();
			context.run();
		}
	} catch(const std::exception& exception) {
		// TODO: exception logic
	}

	return 0;
}