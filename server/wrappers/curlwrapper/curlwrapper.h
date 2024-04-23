#pragma once
#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <string>
#include <initializer_list>

#include <curl/curl.h>

/*

	Enumeration of the request type to the server (web)

*/
enum RequestType
{
	eRT_HTTP,
	eRT_HTTPS
};

/*

	Wrapper for the cURL library for convenient handling of HTTP/S requests
	(Singleton pattern)
*/

class CurlWrapper
{
public:

	// Obtaining a pointer to an object of a class
	static CurlWrapper* getInstance();

	// Fills the curl_slist structure with HTTP/S headers.
	const curl_slist* addHeaders(std::initializer_list<std::string> headers);

	/*
		Perform a request to the web server
		Return value: web server response
	*/
	const std::string performRequest(RequestType type, const std::string& source, const curl_slist* headers);

private:
	CurlWrapper();

	// Handler function for the request callback.
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
};

#endif // !CURL_WRAPPER_H
