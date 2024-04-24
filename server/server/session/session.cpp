#include "session.h"
#include "../api/api.h"

#include "../../logsystem/logmanager/logmanager.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
	LogManager::getInstance()->initEventLog();
	CREATE_EVENT_LOG("Session created")
}

Session::~Session() {
	if (socket.is_open()) {
		socket.close();
	}
}

void Session::run() {
	auto self(shared_from_this());
	this->packetHandler->recvMessage([self](const std::string& message) {
		std::unique_ptr<API> api = std::make_unique<API>(message);
		if (api->isAuthorized()) {
			LogManager::getInstance()->getEventLog()->renameAndMove(api->getUsername());

			self->packetHandler->sendMessage("success_auth");
		}
		else {
			self->packetHandler->sendMessage("failed_auth");
		}
	});
}