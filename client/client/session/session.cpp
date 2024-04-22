#include "session.h"

#include "../api/api.h"

Session::Session(boost::asio::ip::tcp::socket& socket) 
	: socket(std::move(socket)), packetHandler(std::make_unique<PacketHandler>(this->socket)) { }

void Session::run() {
	std::unique_ptr<AuthData> authData = std::make_unique<AuthData>();
	std::unique_ptr<API> api = std::make_unique<API>(authData.get());

	authData->username = "admin";
	authData->password = "aceid";
	authData->hwid = "MjIxOTcwMTY5MTU5MTgxODU4ODI3MDEyMzEzODQxNjlOVklESUEgR2VGb3JjZSBHVFggMTY2MCBUaTM2MTkxMDIyNzAyNTI4MjQ1NTk=";
	authData->token = api->getSessionToken();

	const std::string convertedData = api->convertAuthDataToJson();

	this->packetHandler->sendMessage(convertedData);
}