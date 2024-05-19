#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
#include <boost/asio.hpp>

struct Packet {
    std::size_t size;
    std::vector<char> data;
};

class PacketHandler : public std::enable_shared_from_this<PacketHandler> {
public:

    PacketHandler(boost::asio::ip::tcp::socket& socket);

    void sendMessage(const std::string& message);

    void sendBuffer(const std::vector<char>& buffer);

    void recvMessage(std::function<void(const Packet&)> callback);

    const std::string packetToString(const Packet& packet);
private:

    void sendPacket(const Packet& packet);

    void recvPacket(const Packet& packet, std::function<void(const Packet&)> callback);

private:
    boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
