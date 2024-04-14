#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
#include <optional>
#include <boost/asio.hpp>

struct Packet
{
    std::size_t size;
    std::vector<char> data;
};

class PacketHandler : public std::enable_shared_from_this<PacketHandler>
{
public:
    PacketHandler(boost::asio::ip::tcp::socket& socket);

    void sendMessage(const std::string& message, std::function<void(bool)> callback);
    void sendBuffer(const std::vector<char>& buffer, std::function<void(bool)> callback);

    void recvMessage(std::function<void(const std::string&)> callback);
private:
    void sendPacket(const Packet& packet, std::function<void(bool)> callback);
    void recvPacket(const Packet& packet, std::function<void(const std::string&)> callback);

private:
    boost::asio::ip::tcp::socket socket;
};

#endif // !PACKET_HANDLER_H
