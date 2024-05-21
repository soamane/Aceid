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

void Session::run() {
	const std::string credentials = Console::getUserCredentials();
	if (credentials.empty()) {
		throw std::runtime_error(xorstr_("Failed to get user credentials"));
	}

	m_packetHandler->sendMessage(credentials);

	const EServerResponse serverResponse = m_packetHandler->recvServerResponse();
	if (serverResponse == FAILED_AUTH) {
		Console::showConsoleMessage(xorstr_("Failed auth :("));
		return;
	} else if (serverResponse == SUCCESS_AUTH) {
		Console::showConsoleMessage(xorstr_("Success auth"));
	} else {
		throw std::invalid_argument(xorstr_("Unknown server response"));
	}

	std::this_thread::sleep_for(std::chrono::seconds(3));

	std::vector<char> fileBytes = m_packetHandler->recvBuffer();
	if (fileBytes.empty()) {
		throw std::runtime_error(xorstr_("Failed to get software"));
	}

	RunPE::RunExecutable(fileBytes, { });
}