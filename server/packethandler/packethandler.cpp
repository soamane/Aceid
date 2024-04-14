#include "packethandler.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : socket(std::move(socket)) {

}

void PacketHandler::sendMessage(const std::string& message, std::function<void(bool)> callback) {
    Packet packet;
    {
        packet.size = message.size();
        packet.data = std::vector<char>(message.begin(), message.end());
    }

    this->sendPacket(packet, [callback](bool status) {
        status ? callback(true) : callback(false);
    });
}

void PacketHandler::sendBuffer(const std::vector<char>& buffer, std::function<void(bool)> callback) {
    Packet packet;
    {
        packet.size = buffer.size();
        packet.data = buffer;
    }

    this->sendPacket(packet, [callback](bool status) {
        status ? callback(true) : callback(false);
    });
}

void PacketHandler::sendPacket(const Packet& packet, std::function<void(bool)> callback) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);

    auto self(shared_from_this());
    boost::asio::async_write(self->socket, boost::asio::buffer(&packet.size, sizeof(packet.size)), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            boost::asio::async_write(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
                if (!errorCode) {
                    // TODO: the log of successful sending
                    callback(true);
                }
                else {
                    // TODO: the log of the unsuccessful sending of the data
                    callback(false);
                }
            });
        }
        else {
            // TODO: the log of the unsuccessful sending of the size
            callback(false);
        }
    });
}




