#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <rapidjson/document.h>

struct AuthData {
	std::string username;
	std::string password;
	std::string hwid; 

	std::string token; 
	std::string member_id; 
	std::string profile_group; 
};

class JsonWrapper {
public:

	static JsonWrapper* getInstance();

	const rapidjson::Document parseJsonString(const std::string& jsonString);

	const rapidjson::Value& parseDocumentParams(rapidjson::Document& document);

	bool haveErrorField(const std::string& jsonString);

	bool haveMemberIdField(const std::string& jsonString);

	bool paramsFieldExist(const std::string& jsonString, const std::string& fieldName);

	const std::string parseParamsField(const std::string& jsonString, const std::string& fieldName);

	const AuthData parseUserData(const std::string& jsonString);

	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params);

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H

