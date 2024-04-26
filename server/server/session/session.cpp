#include "session.h"
#include "../api/api.h"

#include "../../logsystem/logmanager/logmanager.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_shared<PacketHandler>(m_socket)) {
	LogManager::getInstance()->initEventLog();
	CREATE_EVENT_LOG("Session created")
}

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
	}
}

void Session::run() {
	auto self(shared_from_this());
	m_packetHandler->recvMessage([self](const std::string& message) {
		std::unique_ptr<API> api = std::make_unique<API>(message);
		if (api->isAuthorized()) {
			LogManager::getInstance()->getEventLog()->renameAndMove(api->getUsername());

			self->m_packetHandler->sendMessage("success_auth");
		}
		else {
			self->m_packetHandler->sendMessage("failed_auth");
		}
	});
}