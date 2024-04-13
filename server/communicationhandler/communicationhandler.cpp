#include "communicationhandler.h"

CommunicationHandler::CommunicationHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}

void CommunicationHandler::SendPacket(const std::string& packet) {

}
