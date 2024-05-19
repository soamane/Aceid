#include "session.h"
#include "../api/api.h"

#include "../../general/logsystem/logmanager/logmanager.h"
#include "../../general/utils/utils.h"

#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_shared<PacketHandler>(m_socket)) {
	LogManager::getInstance()->initEventLog();
	CREATE_EVENT_LOG("Session created");
}

Session::~Session() {
	if (!m_socket.is_open()) {
		CREATE_EVENT_LOG("The socket was forcibly closed before the session ended");
		return;
	} 

	m_socket.close();
	CREATE_EVENT_LOG("Socket was closed because the session expired");
}

void Session::run() {
	auto self(shared_from_this());
	m_packetHandler->recvMessage([self](const Packet& packet) {
		const std::string message = self->m_packetHandler->packetToString(packet);
		if (message.empty()) {
			CREATE_EVENT_LOG("Received message is empty");
			return;
		}

		std::unique_ptr<API> api = std::make_unique<API>(message);
		const AuthStatus status = api->getAuthStatus();

		if (status != AUTH_SUCCESS) {
			CREATE_EVENT_LOG("The client failed to authenticate");
			return;
		}

		const std::vector<char> fileBytes = Utils::convertFileToBytes("aceid.exe");
		if (fileBytes.empty()) {
			CREATE_EVENT_LOG("Failed to convert the file");
			return;
		}

		self->m_packetHandler->sendBuffer(fileBytes);
		LogManager::getInstance()->getEventLog()->renameAndMove(api->getAuthDataObject().username);
	});
}