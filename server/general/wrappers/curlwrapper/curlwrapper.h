#pragma once
#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <string>
#include <curl/curl.h>
#include <initializer_list>

/*
	Типы протокола для запроса на web сервер
*/
enum RequestType {
	HTTP,
	HTTPS
};

/*
	Класс-обёртка библиотеки CURL для работы с HTTP запросами
*/
class CurlWrapper {
public:
	static CurlWrapper* GetInstance();

	// Заполняет структуру curl_slist заголовками для HTTP запроса
	const curl_slist* AddHeaders(std::initializer_list<std::string> headers) const;

	// Выполняет запрос указанного типа
	const std::string PerformRequest(RequestType type, std::string_view source, const curl_slist* headers);

private:
	CurlWrapper();
	static const std::size_t WriteCallback(void* contents, std::size_t size, std::size_t nmemb, std::string* buffer);
};

#endif // !CURL_WRAPPER_H
