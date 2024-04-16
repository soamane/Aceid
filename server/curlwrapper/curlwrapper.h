#pragma once
#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <curl/curl.h>
#include <string>
#include <initializer_list>

enum RequestType
{
	eRT_HTTP,
	eRT_HTTPS
};

class CurlWrapper
{
public:
	static CurlWrapper* getInstance();
	const curl_slist* addHeaders(std::initializer_list<std::string> headers);
	const std::string performRequest(RequestType type, const std::string& source, const curl_slist* headers);

private:
	CurlWrapper();
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
};

#endif // !CURL_WRAPPER_H
