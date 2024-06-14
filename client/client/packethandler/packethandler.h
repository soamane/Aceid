#pragma once
#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

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
	Реализует все необходимые методы общения в синхронном формате
*/
class PacketHandler {
public:
	 // Инициализирует прослушиваемый сокет в поле класса
	PacketHandler(boost::asio::ip::tcp::socket& socket);

	/*
		Метод-обёртка для отправки пакета строкового формата
		Примечание: использует метод SendPacket как основную функцию для отправки данных
	*/
	void SendClientMessage(std::string_view message);

	/*
		Независимый метод для получения ответа сервера
	*/
	const EServerResponse ReceiveServerResponse();

	/*
		Метод-обёртка для чтения данных и конвертацию из общей структуры Packet в строковой тип данных
		Примечание: использует метод ReceivePacket как основную функцию для чтения данных
	*/
	const std::string ReceiveMessage();

	/*
		Метод-обёртка для получения пакета в формате набора символов ( массива )
		Примечание: использует метод ReceivePacket как основную функцию для получения данных
	*/
	const std::vector<char> ReceiveDataBuffer();

private:

	/*
		Основной метод отправки данных
	*/
	void SendPacket(const Packet& packet);

	/*
		Основной метод чтения данных
	*/
	const Packet ReceivePacket();

private:
	boost::asio::ip::tcp::socket m_socket;
};

#endif // !PACKET_HANDLER_H
