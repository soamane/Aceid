#pragma once
#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <string>
#include <initializer_list>

#include <curl/curl.h>

enum RequestType {
	HTTP,
	HTTPS
};

class CurlWrapper {
public:
	static CurlWrapper* GetInstance();

	const curl_slist* AddHeaders(std::initializer_list<std::string> headers) const;
	const std::string PerformRequest(RequestType type, const std::string& source, const curl_slist* headers);

private:
	CurlWrapper();

	static const size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
};

#endif // !CURL_WRAPPER_H
