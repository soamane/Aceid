#include "packethandler.h"

#include <iostream>

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : socket(std::move(socket)) {

}

void PacketHandler::sendMessage(const std::string& message, std::function<void(bool)> callback) {
    Packet packet;
    {
        packet.size = message.size();
        packet.data = std::vector<char>(message.begin(), message.end());
    }

    this->sendPacket(packet, callback);
}

void PacketHandler::sendBuffer(const std::vector<char>& buffer, std::function<void(bool)> callback) {
    Packet packet;
    {
        packet.size = buffer.size();
        packet.data = buffer;
    }

    this->sendPacket(packet, callback);
}

void PacketHandler::recvMessage(std::function<void(const std::string&)> callback) {
    Packet packet;
    {
        this->recvPacket(packet, callback);
    }
}

void PacketHandler::sendPacket(const Packet& packet, std::function<void(bool)> callback) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);

    auto self(shared_from_this());
    boost::asio::async_write(self->socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            boost::asio::async_write(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
                if (!errorCode) {
                    // TODO: Create info log
                    callback(true);
                }
                else {
                    // TODO: Create error log
                    callback(false);
                }
            });
        }
        else {
            // TODO: Create error log
            callback(false);
        }
    });
}

void PacketHandler::recvPacket(const Packet& packet, std::function<void(const std::string&)> callback) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);

    auto self(shared_from_this());
    boost::asio::async_read(self->socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            packetPointer->data.resize(packetPointer->size);
            boost::asio::async_read(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
                if (!errorCode) {
                    // TODO: Create info log
                    std::string result(packetPointer->data.begin(), packetPointer->data.end());
                    callback(result);
                }
                else {
                    // TODO: Create error log
                    callback(std::string());
                }
            });
        }
        else {
            // TODO: Create error log
            callback(std::string());
        }
    });
}





