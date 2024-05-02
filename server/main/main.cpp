#include "../server/server.h"

#include "../general/logsystem/logmanager/logmanager.h"

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
		MessageBoxA(GetForegroundWindow(), "Exception detected, more information check in server.log file", nullptr, MB_OK | MB_ICONERROR);
		CREATE_SERVER_LOG(exception.what())
	}

	return 0;
}