#include "packethandler.h"

#include "../../secure/crypt/crypt.h"
#include "../../logsystem/logmanager/logmanager.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : socket(std::move(socket)) {

}

void PacketHandler::sendMessage(const std::string& message) {
    const std::string encryptedMessage = Crypt::encryptBase64(message);
    Packet packet;
    {
        packet.size = encryptedMessage.size();
        packet.data = std::vector<char>(encryptedMessage.begin(), encryptedMessage.end());
    }

    sendPacket(packet);
}

void PacketHandler::sendBuffer(const std::vector<char>& buffer) {
    Packet packet;
    {
        packet.size = buffer.size();
        packet.data = buffer;
    }

    sendPacket(packet);
}

void PacketHandler::recvMessage(std::function<void(const std::string&)> callback) {
    Packet packet;
    {
        recvPacket(packet, callback);
    }
}

void PacketHandler::sendPacket(const Packet& packet) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);

    auto self(shared_from_this());
    boost::asio::async_write(self->socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            boost::asio::async_write(self->socket, boost::asio::buffer(packetPointer->data), [self, packetPointer](boost::system::error_code errorCode, std::size_t bytes) {
                if (!errorCode) {
                    CREATE_EVENT_LOG("Packet sended without errors: " + std::to_string(bytes) + " bytes sended")
                }
                else {
                    CREATE_EVENT_LOG("Failed to send packet body")
                }
            });
        }
        else {
            CREATE_EVENT_LOG("Failed to send packet size")
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

                    const std::string result(packetPointer->data.begin(), packetPointer->data.end());
                    const std::string decryptedMessage = Crypt::decryptBase64(result);

                    callback(decryptedMessage);
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





