#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <utility>
#include <initializer_list>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class JsonWrapper {
public:
	static JsonWrapper* getInstance();

	const rapidjson::Document parseJsonString(const std::string& jsonString);
	const rapidjson::Value& parseDocumentParams(rapidjson::Document& document);

	bool haveErrorField(const std::string& jsonString);
	bool haveTokenField(const std::string& jsonString);
	bool paramsFieldExist(const std::string& jsonString, const std::string& fieldName);

	const std::string createJsonString(std::initializer_list<std::pair<std::string, std::string>> args, std::initializer_list<std::pair<std::string, std::string>> params);
	const std::string parseSessionToken(const std::string& jsonString);

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
