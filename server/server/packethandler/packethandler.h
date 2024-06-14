#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <memory>
#include <boost/asio.hpp>

/*
    Список возможных ответов от сервера
*/
enum EServerResponse {
    FAILED_AUTH,
    SUCCESS_AUTH
};

/*
    Структура пакета ( единица передаваемой информации )
*/
struct Packet {
    std::size_t size; // Размер данных пакета
    std::vector<char> data; // Несущая информация пакета
};

/*
    Класс обработчик коммуникации с установленным сокетом
    Реализует все необходимые методы общения в асинхронном формате
*/
class PacketHandler : public std::enable_shared_from_this<PacketHandler> {
public:
    // Инициализирует прослушиваемый сокет в поле класса
    PacketHandler(boost::asio::ip::tcp::socket& socket);
    
    /*
        Независимый метод для отправки ответа сервера
    */
    void SendServerResponse(const EServerResponse& response);

    /*
        Метод-обёртка для отправки пакета строкового формата
        Примечание: использует метод SendPacket как основную функцию для отправки данных
    */
    void SendServerMessage(std::string_view message);

    /*
        Метод-обёртка для отправки пакета в формате набора символов ( массива )
        Примечание: использует метод SendPacket как основную функцию для отправки данных
    */
    void SendDataBuffer(const std::vector<char>& buffer); 

    /*
        Метод-обёртка для чтения данных и конвертацию из общей структуры Packet в строковой тип данных
        Примечание: использует метод ReceivePacket как основную функцию для чтения данных
    */
    void ReceiveClientMessage(std::function<void(std::string)> callback);

    /*
        Независимый метод конвертирования пакета в строковой формат
    */
    const std::string ConvertPacketToString(const Packet& packet);

private:
    /*
        Основной метод отправки данных
    */
    void SendPacket(const Packet& packet);

    /*
        Основной метод чтения данных
    */
    void ReceivePacket(std::function<void(const Packet&)> callback);

private:
    boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
