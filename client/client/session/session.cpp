#include "session.h"

#include "../../user/console/console.h"
#include "../../general/utils/utils.h"

#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_unique<PacketHandler>(m_socket)) { }

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
	}
}

void Session::run() {
	const std::string credentials = Console::getUserCredentials();
	m_packetHandler->sendMessage(credentials);

	const EServerResponse serverResponse = m_packetHandler->recvServerResponse();
	if (serverResponse == EServerResponse::eSR_SUCCESS) {
		Console::showConsoleMessage("success auth");

		std::vector<char> fileBytes = m_packetHandler->recvBuffer();
		Utils::createFileFromBytes("getted.jpeg", fileBytes);
	}
	else {
		Console::showConsoleMessage("failed auth");
	}

	Sleep(-1);
}