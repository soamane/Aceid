#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <utility>
#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

/*
	Класс-обёртка библиотеки RapidJSON
	Специализирован под обработку данных JSON формата и конвертирование данных в JSON формат
*/
class JsonWrapper {
public:
	static JsonWrapper* GetInstance();

	// Конвертирует поступаемую строку в JSON документ
	const rapidjson::Document ConvertStringToJson(std::string_view jsonString) const;

	// Получает объект "params" в JSON документе
	const rapidjson::Value& ParseDocumentParams(rapidjson::Document& document) const;

	// Проверяет существование поля "error" в объекте "params"
	const bool IsErrorField(std::string_view jsonString) const;

	// Проверяет существование поля "token" в объекте "params"
	const bool haveTokenField(std::string_view jsonString) const;

	// Проверяет существования fieldName поля в объекте "params" 
	const bool ParamsFieldExist(std::string_view jsonString, std::string_view fieldName) const;

	// Конвертирует поступаемые аргументы в JSON формат строку
	const std::string CreateJsonString(const std::unordered_map<std::string, std::string>& args, const std::unordered_map<std::string, std::string>& params) const;

	// Парсит поле"token" из объекта "params" 
	const std::string ParseSessionToken(std::string_view jsonString) const;

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
