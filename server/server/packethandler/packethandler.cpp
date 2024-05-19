#include "packethandler.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

PacketHandler::PacketHandler(boost::asio::ip::tcp::socket& socket)
    : m_socket(std::move(socket)) {

}

void PacketHandler::sendMessage(const std::string& message) {
    if (message.empty()) {
        throw std::invalid_argument("Function call error: empty argument (message)");
    }

    const std::string encryptedMessage = DataEncryption::encryptCustomMethod(message);
    if (encryptedMessage.empty()) {
        CREATE_EVENT_LOG("Failed to initialize encrypted message");
        return;
    }

    const std::size_t messageSize = encryptedMessage.size();
    const std::vector<char> messageData = std::vector<char>(encryptedMessage.begin(), encryptedMessage.end());

    Packet packet;
    {
        packet.size = messageSize;
        packet.data = messageData;
    }

    sendPacket(packet);
}

void PacketHandler::sendBuffer(const std::vector<char>& buffer) {
    if (buffer.empty()) {
        throw std::invalid_argument("Function call error: empty argument (buffer)");
    }

    const std::vector<char> encryptedBuffer = DataEncryption::encryptBuffer(buffer);
    if (encryptedBuffer.empty()) {
        CREATE_EVENT_LOG("Failed to initialize encrypted buffer");
        return;
    }

    const std::size_t bufferSize = encryptedBuffer.size();

    Packet packet;
    {
        packet.size = bufferSize;
        packet.data = encryptedBuffer;
    }

    sendPacket(packet);
}

void PacketHandler::recvMessage(std::function<void(const Packet&)> callback) {
    Packet packet;
    {
        recvPacket(packet, callback);
    }
}

const std::string PacketHandler::packetToString(const Packet& packet) {
    if (packet.data.empty()) {
        CREATE_EVENT_LOG("Failed to convert packet to string");
        return std::string();
    }

    const std::string message(packet.data.begin(), packet.data.end());
    if (message.empty()) {
        CREATE_EVENT_LOG("Failed to convert the result string");
        return std::string();

    }

    const std::string decryptedMessage = DataEncryption::decryptCustomMethod(message);
    if (decryptedMessage.empty()) {
        CREATE_EVENT_LOG("Failed to decrypt string");
        return std::string();
    }

    return decryptedMessage;
}

void PacketHandler::sendPacket(const Packet& packet) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);
    if (!packetPointer) {
        CREATE_EVENT_LOG("Failed to initialize packet pointer (send)");
        return;
    }

    auto self(shared_from_this());
    boost::asio::async_write(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer](boost::system::error_code errorCode, std::size_t) {
        if (errorCode) {
            CREATE_EVENT_LOG("Failed to send packet size");
            return;
        } 

        boost::asio::async_write(self->m_socket, boost::asio::buffer(packetPointer->data), [self, packetPointer](boost::system::error_code errorCode, std::size_t bytes) {
            if (errorCode) {
                CREATE_EVENT_LOG("Failed to send packet body");
                return;
            } 

            CREATE_EVENT_LOG("Packet sended without errors: " + std::to_string(bytes) + " bytes sended");
        });
    });
}

void PacketHandler::recvPacket(const Packet& packet, std::function<void(const Packet&)> callback) {
    std::shared_ptr<Packet> packetPointer = std::make_shared<Packet>(packet);
    if (!packetPointer) {
        CREATE_EVENT_LOG("Failed to initialize packet pointer (recv)");
        return;
    }

    auto self(shared_from_this());
    boost::asio::async_read(self->m_socket, boost::asio::buffer(&packetPointer->size, sizeof(packetPointer->size)), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t) {
        if (errorCode) {
            CREATE_EVENT_LOG("Failed to send packet size");
            callback(Packet());
        } 
        
        try {
            packetPointer->data.resize(packetPointer->size);
        } catch (const std::bad_alloc& e) {
            CREATE_EVENT_LOG(e.what());
            callback(Packet());
        }
        
        boost::asio::async_read(self->m_socket, boost::asio::buffer(packetPointer->data), [self, packetPointer, callback](boost::system::error_code errorCode, std::size_t bytes) {
            if (errorCode) {
                CREATE_EVENT_LOG("Failed to recv packet body");
                callback(Packet());
            } 

            CREATE_EVENT_LOG("Packet received without errors: " + std::to_string(bytes) + " bytes received");

            callback(*packetPointer);
        });
    });
}