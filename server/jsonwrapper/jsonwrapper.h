#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

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

	bool isErrorField(const std::string& jsonString);

	const AuthData parseAuthData(const std::string& jsonString);
	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params);
private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
