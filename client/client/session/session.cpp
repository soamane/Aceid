#include "session.h"

#include "../../app/console/console.h"
#include "../../general/utils/utils.h"

#include "../../app/runpe/runpe.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/protect/xorstring/xorstring.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_unique<PacketHandler>(m_socket)) { }

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
	}
}

void Session::Run() {
	const std::string credentials = Console::GetUserCredentials();
	if (credentials.empty()) {
		throw std::runtime_error(xorstr_("Failed to get user credentials"));
	}

	m_packetHandler->SendClientMessage(credentials);

	const EServerResponse serverResponse = m_packetHandler->ReceiveServerResponse();
	switch (serverResponse) {
		case FAILED_AUTH:
			Console::PrintConsoleMessage(xorstr_("FAILED AUTH"));
			std::this_thread::sleep_for(std::chrono::seconds(3));
			return;

		case SUCCESS_AUTH:
			Console::PrintConsoleMessage(xorstr_("SUCCESS AUTH "));
			break;

		default:
			throw std::invalid_argument(xorstr_("Unknown server response"));
	}

	std::vector<char> fileBytes = m_packetHandler->ReceiveDataBuffer();
	if (fileBytes.empty()) {
		throw std::runtime_error(xorstr_("Failed to get software"));
	}

	Console::PrintConsoleMessage(xorstr_("OPEN GAME FOR CHEAT INJECTION\n"));
	std::this_thread::sleep_for(std::chrono::seconds(5));
	Console::Clear();

	RunPE::RunExecutable(fileBytes, { });
}