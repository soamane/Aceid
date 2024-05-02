#include "session.h"
#include "../api/api.h"

#include "../../general/logsystem/logmanager/logmanager.h"
#include "../../general/utils/utils.h"

#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_shared<PacketHandler>(m_socket)) {
	LogManager::getInstance()->initEventLog();
	CREATE_EVENT_LOG("Session created")
}

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
	}

	CREATE_EVENT_LOG("Session closed")
}

void Session::run() {
	auto self(shared_from_this());
	m_packetHandler->recvMessage([self](const std::string& message) {
		std::unique_ptr<API> api = std::make_unique<API>(message);
		if (!api->isAuthorized()) {
			return;
		}

		const std::vector<char> fileBytes = Utils::convertFileToBytes("test.exe");
		self->m_packetHandler->sendBuffer(fileBytes);

		LogManager::getInstance()->getEventLog()->renameAndMove(api->getUsername());
	});
}