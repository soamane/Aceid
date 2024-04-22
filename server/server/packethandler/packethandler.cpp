#include "packethandler.h"

#include "../../logsystem/logmanager/logmanager.h"

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
            boost::asio::async_write(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t bytes) {
                if (!errorCode) {
                    CREATE_EVENT_LOG("Packet sended without errors: " + std::to_string(bytes) + " bytes sended")
                    callback(true);
                }
                else {
                    CREATE_EVENT_LOG("Failed to send packet body")
                    callback(false);
                }
            });
        }
        else {
            CREATE_EVENT_LOG("Failed to send packet size")
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
            boost::asio::async_read(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t bytes) {
                if (!errorCode) {
                    CREATE_EVENT_LOG("Packet received without errors: " + std::to_string(bytes) + " bytes received")

                    std::string result(packetPointer->data.begin(), packetPointer->data.end());
                    callback(result);
                }
                else {
                    CREATE_EVENT_LOG("Failed to recv packet body")
                    callback(std::string());
                }
            });
        }
        else {
            CREATE_EVENT_LOG("Failed to send packet size")
            callback(std::string());
        }
    });
}





