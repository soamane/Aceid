#pragma once
#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <rapidjson/document.h>

struct AuthData
{
	std::string username;
	std::string password;
	std::string hwid;
};

class JsonParser
{
public:
	static JsonParser* getInstance();

	const AuthData parseAuthData(const std::string& jsonString);

private:
	JsonParser();
};

#endif // !JSON_PARSER_H
