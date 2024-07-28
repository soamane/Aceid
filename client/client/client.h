#pragma once
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <boost/asio.hpp>

/*
	Основной класс приложения для подключения к серверу
*/
class Client {
public:
	// Инициализирует поля класса
	Client(boost::asio::io_context& context);

	/*
		Подключается к серверу по установленному адресу и порту
		Примечание: при успешном соединении создает объект сессии и открывает ее
	*/
	void Connect(std::string_view address, std::string_view port);

private:
	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::ip::tcp::socket m_socket;
};

#endif // !CONNECTOR_H
