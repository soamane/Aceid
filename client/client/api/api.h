#pragma once
#ifndef API_H
#define API_H

#include <memory>
#include <string>

#include "../../general/protect/xorstring/xorstring.h"
#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

/*
	Структура данных клиента для аутентификации
*/
struct AuthData {
	std::string username;
	std::string password;
	std::string hwid;

	std::string token;
	std::string launchParams;
};

/*
	Класс для обработки и получения данных API
	Используется обращение к API сайта
*/
class API {
public:

	// Инициализирует структуру данных пользователя
	API(AuthData* data);

	// Конвертирует структуру данных пользователя в строку JSON формата
	const std::string ConvertAuthDataToJson();

	// Создаёт и возвращает строку JSON формата для запуска полученного файла с сервера
	const std::string CreateLaunchParams();

	// Обращается к Web API и возвращает полученный токен сессии
	const std::string GetSessionToken();

private:
	/*
		Выполняет HTTP API запрос по сформированному json формату url
	*/
	const std::string PerformRequestToGetSessionToken(std::string_view jsonString);

private:
	AuthData* m_authData; // Объект структуры данных пользователя
	const std::string m_url = xorstr_("https://aceid.cc/server_api/api.php"); // Адрес Web API
};

#endif // !API_H
