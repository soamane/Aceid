#include "packethandler.h"

#include "../../app/protect/dataencryption/dataencryption.h"
#include "../../app/logsystem/logmanager/logmanager.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : m_socket(std::move(socket)) {

}

void PacketHandler::sendMessage(const std::string& message) {
    const std::string encryptedMessage = DataEncryption::encryptCustomMethod(message);
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

void PacketHandler::sendServerResponse(const EServerResponse& response) {
    auto self(shared_from_this());
    boost::asio::async_write(self->m_socket, boost::asio::buffer(&response, sizeof(response)), [self](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            CREATE_EVENT_LOG("Server response sended without errors")
        }
        else {
            CREATE_EVENT_LOG("Failed to send server response")
        }
    });
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
    boost::asio::async_write(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            boost::asio::async_write(self->m_socket, boost::asio::buffer(packetPointer->data), [self, packetPointer](boost::system::error_code errorCode, std::size_t bytes) {
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
    boost::asio::async_read(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (!errorCode) {
            packetPointer->data.resize(packetPointer->size);
            boost::asio::async_read(self->m_socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t bytes) {
                if (!errorCode) {
                    CREATE_EVENT_LOG("Packet received without errors: " + std::to_string(bytes) + " bytes received")

                    const std::string result(packetPointer->data.begin(), packetPointer->data.end());
                    const std::string decryptedMessage = DataEncryption::decryptCustomMethod(result);

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





