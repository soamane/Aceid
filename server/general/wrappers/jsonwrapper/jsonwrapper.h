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
	static JsonWrapper* GetInstance();

	const rapidjson::Document ConvertStringToJson(const std::string& jsonString) const;
	const rapidjson::Value& ParseDocumentParams(rapidjson::Document& document) const;

	const bool IsErrorField(const std::string& jsonString) const;
	const bool IsMemberIdField(const std::string& jsonString) const;
	const bool ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const;

	const std::string ParseParamsField(const std::string& jsonString, const std::string& fieldName) const;
	const AuthData ParseUserData(const std::string& jsonString) const;

	const std::string CreateJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params) const;

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H

