#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
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

    void sendPacket(const Packet& packet);

private:
    boost::asio::ip::tcp::socket socket;
};

#endif // !PACKET_HANDLER_H
