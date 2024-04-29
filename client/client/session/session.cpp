#include "session.h"

#include "../api/api.h"
#include "../../hardware/hardware.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_unique<PacketHandler>(m_socket)) { }

void Session::run() {
	std::unique_ptr<AuthData> authData = std::make_unique<AuthData>();
	std::unique_ptr<API> api = std::make_unique<API>(authData.get());

	std::cout << "username: ";
	std::cin >> authData->username;

	std::cout << "password: ";
	std::cin >> authData->password;

	authData->hwid = Hardware::getHardwareId();
	authData->token = api->getSessionToken();

	const std::string convertedAuthData = api->convertAuthDataToJson();
	m_packetHandler->sendMessage(convertedAuthData);

	const std::string serverResponse = m_packetHandler->recvMessage();
	std::cout << "server response: " << serverResponse << std::endl;

	Sleep(-1);
}