#pragma once
#ifndef SESSION_H
#define SESSION_H

#include "../packethandler/packethandler.h"

/*
    Класс для построения логической связи между клиентом и сервером.
    Реализует уникальную сессию для каждого подключившегося сокета.
*/
class Session : public std::enable_shared_from_this<Session> {
public:
    /*
        Инициализирует поле сокета и класс обработчика коммуникации.
    */
    explicit Session(boost::asio::ip::tcp::socket& socket);

    /*
        Закрывает прослушиваемый сокет и
        освобождает указатель на класс обработчика коммуникации.
    */
    ~Session();

    /*
        Основная функция сессии.
    */
    void Open();

private:

    /*
        Сессионный обработчик поступаемых данных

        Действия:
        1. Запускает обработку полученных данных на уровне API сайта
        2. Валидирует конечный статус авторизации
        3. При успешной авторизации отправляет необходимые данные клиенту
           * в случае безуспешной авторизации отправляет только отклоненный ответ
    */
    void HandleClientMessage(std::string_view jsonData);

private:
    boost::asio::ip::tcp::socket m_socket; // Прослушиваемый сокет
    std::shared_ptr<PacketHandler> m_packetHandler; // Обработчик коммуникации с сокетом
};

#endif // !SESSION_H
