#pragma once
#ifndef COMMUNICATION_HANDLER_H
#define COMMUNICATION_HANDLER_H

#include <memory>
#include <boost/asio.hpp>

struct Packet
{
    std::size_t size;
    std::vector<char> data;
};

class CommunicationHandler : public std::enable_shared_from_this<CommunicationHandler>
{
public:
    CommunicationHandler(boost::asio::ip::tcp::socket& socket);

    void sendPacket(const Packet& packet);

private:
    boost::asio::ip::tcp::socket socket;
};

#endif // !COMMUNICATION_HANDLER_H
