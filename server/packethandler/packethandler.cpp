#include "packethandler.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : socket(std::move(socket)) {

}

void PacketHandler::sendPacket(const Packet& packet) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);

    auto self(shared_from_this());
    boost::asio::async_write(self->socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            boost::asio::async_write(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer](boost::system::error_code errorCode, std::size_t bytes) {
                if (!errorCode) {
                    // TODO: the log of successful sending
                }
                else {
                    // TODO: the log of the unsuccessful sending of the data
                }
                });
        }
        else {
            // TODO: the log of the unsuccessful sending of the size
        }
        });
}




