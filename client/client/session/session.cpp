#include "session.h"

#include "../../user/console/console.h"

#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket)
	: m_socket(std::move(socket)), m_packetHandler(std::make_unique<PacketHandler>(m_socket)) { }

void Session::run() {
	const std::string credentials = Console::getUserCredentials();
	m_packetHandler->sendMessage(credentials);

	const std::string serverResponse = m_packetHandler->recvMessage();
	std::cout << "server response: " << serverResponse << std::endl;

	Sleep(-1);
}