#include "session.h"
#include "../api/api.h"

#include "../../general/logsystem/logmanager/logmanager.h"
#include "../../general/utils/utils.h"

#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket) : m_socket(std::move(socket)), m_packetHandler(std::make_shared<PacketHandler>(m_socket)) {
	LogManager::GetInstance()->InitEventLog();
	CREATE_EVENT_LOG("Session created");
}

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
		CREATE_EVENT_LOG("Socket was closed because the session expired");
		return;
	}

	CREATE_EVENT_LOG("Session closed");
}


void Session::Open() {
	auto self(shared_from_this());
	m_packetHandler->ReceiveClientMessage([self](const std::string& message) {
		if (message.empty()) {
			CREATE_EVENT_LOG("Received message is empty");
			return;
		}

		API api(message);

		const AuthStatus authStatus = api.GetAuthStatus();
		if (authStatus == AUTH_SUCCESS) {
			CREATE_EVENT_LOG("Client has successfully logged in");
			LogManager::GetInstance()->GetEventLog()->RenameAndMove(api.GetAuthDataObject().username); // rename event log file

			self->m_packetHandler->SendServerResponse(SUCCESS_AUTH);

			const std::vector<char>& fileBytes = Utils::ConvertFileToBytes("aceid.exe");
			if (fileBytes.empty()) {
				CREATE_EVENT_LOG("Failed to convert the file");
				return;
			}

			self->m_packetHandler->SendDataBuffer(fileBytes);
		} else {
			CREATE_EVENT_LOG("Client failed to authenticate");
			self->m_packetHandler->SendServerResponse(FAILED_AUTH);
			return;
		}
	});
}