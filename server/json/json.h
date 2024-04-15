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

class Json
{
public:
	static Json* getInstance();

	const AuthData parseAuthData(const std::string& jsonString);

private:
	Json();
};

#endif // !JSON_PARSER_H
