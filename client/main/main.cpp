#include "../client/client.h"

#include "../app/app.h"
#include "../general/protect/xorstring/xorstring.h"

int main() {
	try {
		Application app;
		app.createApplicationMutex();

		boost::asio::io_context context;
		Client client(context);
		{
			client.connect(xorstr_("31.129.59.129"), xorstr_("17521"));
		}
	} catch (const std::exception& exception) {
		MessageBoxA(GetForegroundWindow(), exception.what(), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}