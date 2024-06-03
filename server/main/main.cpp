#include "../server/server.h"

#include "../general/logsystem/logmanager/logmanager.h"

int main() {
	try {
		boost::asio::io_context context;
		Server server(context, 17522);
		{
			server.Start();
			context.run();
		}
	} catch (const std::exception& exception) {
		CREATE_SERVER_LOG(exception.what());
		MessageBoxA(GetForegroundWindow(), "Exception detected, more information check in server log file", nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}