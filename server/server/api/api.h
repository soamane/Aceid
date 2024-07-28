#pragma once
#ifndef API_H
#define API_H

#include <optional>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"

/*
    Список возможных статусов авторизации
    по итогам обработки с помощью API
*/
enum AuthStatus {
    AUTH_INVALID_CREDENTIALS,
    AUTH_INVALID_HWID,
    AUTH_INVALID_LICENSE,
    AUTH_INVALID_TOKEN,
    AUTH_SUCCESS
};

/*
    Класс для обработки поступаемых данных клиента
    Используется обращение к API сайта
*/
class API {
public:
    /*
        1. Вызывает метод для получения общей структуры данных пользователя
        2. Вызывает метод для получения memberId
        3. Вызывает метод для получения profileGroupId (дополнительных подписок)
    */
    API(std::string_view jsonString);

    /*
        Возвращает итоговый результат обработки данных

        Действия:
        1. Вызывает метод проверки авторизации
        2. Вызывает метод проверки HWID
        3. Вызывает метод проверки лицензии
        4. Вызывает метод проверки токена сессии
    */
    const AuthStatus GetAuthStatus() const;

    /*
        Возвращает обрабатываемый объект структуры данных пользователя
    */
    const AuthData& GetAuthDataObject() const;

private:
    // Метод для получения общей структуры данных пользователя
    void GetUserCredentials(std::string_view jsonString);

    // Метод для получения profileGroupId (дополнительных подписок)
    void GetProfileGroupId();

    // Метод для получения memberId
    void GetMemberId();

    /*
        Все ниже перечисленные функции конвертируют определенный вид json-строки
                            для запроса на web-сервер
    */
    const bool CheckUserAuthentication() const;
    const bool CheckUserHwid() const;
    const bool CheckUserLicense() const;
    const bool CheckUserToken() const;

    /*
        Выполняет HTTP API запрос по сформированному json формату url
    */
    std::optional<std::string> PerformApiRequest(std::string_view jsonString) const;

private:
    AuthData m_authData; // Объект структуры данных пользователя
    const std::string m_url = "https://aceid.cc/server_api/api.php"; // Адрес Web API
};

#endif // !API_H
