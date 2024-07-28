#pragma once
#ifndef SESSION_H
#define SESSION_H

#include "../packethandler/packethandler.h"

/*
	Класс для построения логической связи между клиентом и сервером.
	Реализует уникальную сессию для общения с сервером.
*/
class Session {
public:
	/*
		Инициализирует поле сокета и класс обработчика коммуникации.
	*/
	Session(boost::asio::ip::tcp::socket& socket);

	/*
		Вызывает метод закрытия сессии
	*/
	~Session();

	/*
		Основная функция сессии.
	*/
	void Open();

	/*
		Закрывает прослушиваемый сокет и
		освобождает указатель на класс обработчика коммуникации.
	*/
	void Close();

private:
	// Инициализирует отдельный поток для установки времени сессии
	void InitTimeoutThread();

	// Метод для установки таймаута
	void TimeoutFunction();

private:
	boost::asio::ip::tcp::socket m_socket; // Прослушиваемый сокет
	std::unique_ptr<PacketHandler> m_packetHandler; // Обработчик коммуникации с сокетом
};

#endif // !SESSION_H
