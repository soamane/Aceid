#include "../client/client.h"
#include "../general/protect/xorstring/xorstring.h"

int main() {
	try {
		boost::asio::io_context context;
		Client client(context);
		{
			client.Connect(xorstr_("195.133.49.126"), xorstr_("17522"));
		}
	} catch (const std::exception& exception) {
		MessageBoxA(GetForegroundWindow(), exception.what(), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}