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

	std::string token;
	std::string member_id;
};

class JsonWrapper
{
public:
	static JsonWrapper* getInstance();

	bool haveErrorField(const std::string& jsonString);
	bool haveMemberIdField(const std::string& jsonString);

	const std::string parseMemberId(const std::string& jsonString);
	const AuthData parseUserData(const std::string& jsonString);

	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params);
private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
