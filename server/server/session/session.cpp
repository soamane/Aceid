#include "session.h"
#include "../api/api.h"

#include "../../logsystem/logmanager/logmanager.h"

Session::Session(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)), packetHandler(std::make_shared<PacketHandler>(this->socket)) {
	LogManager::getInstance()->initEventLog();
	CREATE_EVENT_LOG("Session created")
}

Session::~Session() {
	if (socket.is_open()) {
		socket.close();
		CREATE_EVENT_LOG("Socket closed")
	}

	CREATE_EVENT_LOG("Session closed")
}

void Session::run() {
	auto self(shared_from_this());
	this->packetHandler->recvMessage([self](const std::string& message) {
		API api = API(message);
		if (api.isAuthorized()) {
			CREATE_EVENT_LOG("Success auth")
		}
		else {
			CREATE_EVENT_LOG("Authorization error")
		}
	});
}