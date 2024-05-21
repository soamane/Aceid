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
	if (m_socket.is_open()) {
		m_socket.close();
		CREATE_EVENT_LOG("Socket was closed because the session expired");
		return;
	} 
}

void Session::run() {
	auto self(shared_from_this());
	m_packetHandler->recvMessage([self](const std::string& message) {
		if (message.empty()) {
			CREATE_EVENT_LOG("Received message is empty");
			return;
		}

		API api(message);

		const AuthStatus authStatus = api.getAuthStatus();
		if (authStatus != AUTH_SUCCESS) {
			CREATE_EVENT_LOG("Client failed to authenticate");
			self->m_packetHandler->sendServerResponse(EServerResponse::ERROR_RESPONSE);
			return;
		}

		self->m_packetHandler->sendServerResponse(EServerResponse::SUCCESS_RESPONSE);

		const std::vector<char> fileBytes = Utils::convertFileToBytes("aceid.exe");
		if (fileBytes.empty()) {
			CREATE_EVENT_LOG("Failed to convert the file");
			return;
		}

		self->m_packetHandler->sendBuffer(fileBytes);
		LogManager::getInstance()->getEventLog()->renameAndMove(api.getAuthDataObject().username);
	});
}