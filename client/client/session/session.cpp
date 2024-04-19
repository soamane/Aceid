#include "session.h"

#include "../api/api.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {
	std::unique_ptr<AuthData> data = std::make_unique<AuthData>();
	std::unique_ptr<API> api = std::make_unique<API>(data.get());

	data->username = "username_value";
	data->password = "password_value";
	data->hwid = "hwid_value";
	data->token = api->getSessionToken();

	std::string request = api->convertAuthDataToJson();
	if (!this->packetHandler->sendMessage(request)) {
		// TODO: error log
		return;
	}

	Sleep(-1);
}