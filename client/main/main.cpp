#include "../client/client.h"

#include "../user/user.h"
#include "../general/protect/xorstring/xorstring.h"

int main() {
	try {
		User user;
		user.createApplicationMutex();

		boost::asio::io_context context;
		Client client(context);
		{
			client.connect(xorstr_("31.129.43.210"), xorstr_("25565"));
		}
	} catch (const std::exception& exception) {
		MessageBoxA(GetForegroundWindow(), exception.what(), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}