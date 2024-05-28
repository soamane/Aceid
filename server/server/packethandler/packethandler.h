#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
#include <boost/asio.hpp>

enum EServerResponse {
    FAILED_AUTH,
    SUCCESS_AUTH
};

struct Packet {
    std::size_t size;
    std::vector<char> data;
};

class PacketHandler : public std::enable_shared_from_this<PacketHandler> {
public:
    PacketHandler(boost::asio::ip::tcp::socket& socket);
    
    void SendServerResponse(const EServerResponse& response);
    void SendServerMessage(const std::string& message);
    void SendDataBuffer(const std::vector<char>& buffer); 

    void ReceiveClientMessage(std::function<void(const std::string&)> callback);

    const std::string ConvertPacketToString(const Packet& packet);

private:
    void SendPacket(const Packet& packet);
    void ReceivePacket(std::function<void(const Packet&)> callback);

private:
    boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
