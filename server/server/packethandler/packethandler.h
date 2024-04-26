#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
#include <boost/asio.hpp>

/*

    Packet structure for network communication

*/
struct Packet
{
    std::size_t size;
    std::vector<char> data;
};

/*

    TCP/IP communication handler class

*/
class PacketHandler : public std::enable_shared_from_this<PacketHandler>
{
public:
    // Initializes the socket
    PacketHandler(boost::asio::ip::tcp::socket& socket);

    // Sends a packet of type std::string
    void sendMessage(const std::string& message);

    // Sends a packet of type std::vector<char>
    void sendBuffer(const std::vector<char>& buffer);

    /*
        Reads the received packet from the client
        Return value: callback
    */
    void recvMessage(std::function<void(const std::string&)> callback);

private:
    // Function for sending a packet structure
    void sendPacket(const Packet& packet);

    /*
        Packet reading function
        Return value: callback
    */
    void recvPacket(const Packet& packet, std::function<void(const std::string&)> callback);

private:
    boost::asio::ip::tcp::socket m_socket; // current remote socket
};

#endif // !PACKET_HANDLER_H
