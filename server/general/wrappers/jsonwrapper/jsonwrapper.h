#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <unordered_map>

#include <rapidjson/document.h>

/*
	Структура данных клиента для аутентификации
*/
struct AuthData {
	std::string username;
	std::string password; 
	std::string hwid; // Уникальный ID компьютера 

	std::string token; // Токен текущей сессии с сервером
	std::string member_id; 
	std::string profile_group; 
};

/*
	Класс-обёртка библиотеки RapidJSON
	Обрабатывает, конвертирует и парсит необходимые данные
	Примечание: специализирован под обработку данных поступаемых в качестве ответа от web-сервера и данных клиента
*/
class JsonWrapper {
public:
	static JsonWrapper* GetInstance();

	// Конвертирует поступаемую строку в документ JSON формата 
	const rapidjson::Document ConvertStringToJson(const std::string& jsonString) const;

	// Возвращает поле "params" в обрабатываемом документе JSON
	const rapidjson::Value& ParseDocumentParams(rapidjson::Document& document) const;

	// Проверяет наличие поле "error" в полях JSON строки
	const bool IsErrorField(const std::string& jsonString) const;

	// Проверяет наличие поля "id" в полях JSON строки
	const bool IsMemberIdField(const std::string& jsonString) const;

	// Проверяет существование поля с указанным именем 
	const bool ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const;

	// Парсит данные из указанного поля в аргументах "params"
	const std::string ParseParamsField(const std::string& jsonString, const std::string& fieldName) const;

	/*
		Парсит учётные данные пользователя в структуру AuthData
		Примечание: на вход поступает строка JSON формата от клиента (учётные данные)
	*/
	const AuthData ParseUserData(const std::string& jsonString) const;

	/*
		Конвертирует аргументы функции в строку JSON формата
		Поддерживает добавление заголовков (аргументов)
	*/
	const std::string CreateJsonString(const std::unordered_map<std::string, std::string>& args, const std::unordered_map<std::string, std::string>& params) const;

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H

