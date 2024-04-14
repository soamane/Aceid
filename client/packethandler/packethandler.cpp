#include "packethandler.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
	: socket(std::move(socket)) {

}