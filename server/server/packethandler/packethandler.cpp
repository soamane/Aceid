#include "packethandler.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket) : m_socket(std::move(socket)) { }

void PacketHandler::SendServerResponse(const EServerResponse& response) {
    boost::asio::async_write(m_socket, boost::asio::buffer(&response, sizeof(response)),
                             [](boost::system::error_code errorCode, std::size_t) {
        if (errorCode) {
            CREATE_EVENT_LOG("Failed to send server response: " + errorCode.message());
        } else {
            CREATE_EVENT_LOG("Server response sent successfully");
        }
    });
}

void PacketHandler::SendServerMessage(const std::string& message) {
    if (message.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    // Шифрование исходной строки для дальнейшей передачи
    const std::string encryptedMessage = DataEncryption::EncryptCustomMethod(message);
    if (encryptedMessage.empty()) {
        CREATE_EVENT_LOG("Failed to encrypt message");
        return;
    }

    SendPacket({ encryptedMessage.size(), std::vector<char>(encryptedMessage.begin(), encryptedMessage.end()) });
}

void PacketHandler::SendDataBuffer(const std::vector<char>& buffer) {
    if (buffer.empty()) {
        CREATE_EVENT_LOG("Function call error: empty argument [" + std::string(__func__) + "]");
        return;
    }

    // Шифрование массива символа для дальнейшей передачи
    const std::vector<char> encryptedBuffer = DataEncryption::EncryptBuffer(buffer);
    if (encryptedBuffer.empty()) {
        CREATE_EVENT_LOG("Failed to encrypt buffer");
        return;
    }

    SendPacket({ encryptedBuffer.size(), encryptedBuffer });
}

void PacketHandler::ReceiveClientMessage(std::function<void(const std::string&)> callback) {
    ReceivePacket([this, callback](const Packet& packet) {
        const std::string message = ConvertPacketToString(packet);
        if (message.empty()) {
            CREATE_EVENT_LOG("Failed to convert packet to string");
            return;
        }

        callback(message);
    });
}

const std::string PacketHandler::ConvertPacketToString(const Packet& packet) {
    if (packet.data.empty() || packet.size != packet.data.size()) {
        CREATE_EVENT_LOG("Function call error: invalid packet data in [" + std::string(__func__) + "]");
        return {};
    }

    // Расшифровка конвертируемого пакета
    const std::string decryptedMessage = DataEncryption::DecryptCustomMethod(std::string(packet.data.begin(), packet.data.end()));
    if (decryptedMessage.empty()) {
        CREATE_EVENT_LOG("Failed to decrypt message string");
        return {};
    }

    return decryptedMessage;
}

void PacketHandler::SendPacket(const Packet& packet) {
    auto self(shared_from_this());
    auto packetPointer = std::make_shared<Packet>(packet);

    boost::asio::async_write(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)),
                             [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
        if (errorCode) {
            CREATE_EVENT_LOG("Failed to send packet size: " + errorCode.message());
            return;
        }

        boost::asio::async_write(self->m_socket, boost::asio::buffer(packetPointer->data),
                                 [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
            if (errorCode) {
                CREATE_EVENT_LOG("Failed to send packet body: " + errorCode.message());
            } else {
                CREATE_EVENT_LOG("Packet sent without errors");
            }
        });
    });
}

void PacketHandler::ReceivePacket(std::function<void(const Packet&)> callback) {
    auto self(shared_from_this());
    auto packetPointer = std::make_shared<Packet>();

    boost::asio::async_read(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)),
                            [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (errorCode) {
            CREATE_EVENT_LOG("Failed to receive packet size: " + errorCode.message());
            callback(Packet());
        }

        try {
            packetPointer->data.resize(packetPointer->size);
        } catch (const std::bad_alloc& e) {
            CREATE_EVENT_LOG("Memory allocation error: " + std::string(e.what()));
            callback(Packet());
        }

        boost::asio::async_read(self->m_socket, boost::asio::buffer(packetPointer->data),
                                [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
            if (errorCode) {
                CREATE_EVENT_LOG("Failed to receive packet body: " + errorCode.message());
                callback(Packet());
            } else {
                CREATE_EVENT_LOG("Packet received without errors");
                callback(*packetPointer);
            }
        });
    });
}
