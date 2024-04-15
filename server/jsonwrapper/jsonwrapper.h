#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <rapidjson/document.h>

struct AuthData
{
	std::string username;
	std::string password;
	std::string hwid;
};

class JsonWrapper
{
public:
	static JsonWrapper* getInstance();

	const AuthData parseAuthData(const std::string& jsonString);

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
