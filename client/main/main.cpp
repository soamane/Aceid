#include "../client/client.h"

int main( )
{
	try
	{
		boost::asio::io_context context;
		Client client(context);
		{
			client.connect("31.129.43.210", "25565");
		}
	} catch (const std::exception& exception)
	{
		MessageBoxA(GetForegroundWindow( ), exception.what( ), nullptr, MB_OK | MB_ICONERROR);
	}

	return 0;
}