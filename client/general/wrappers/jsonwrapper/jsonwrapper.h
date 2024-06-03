#pragma once
#ifndef JSON_WRAPPER_H
#define JSON_WRAPPER_H

#include <string>
#include <utility>
#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

class JsonWrapper {
public:
	static JsonWrapper* GetInstance();

	const rapidjson::Document ConvertStringToJson(const std::string& jsonString) const;
	const rapidjson::Value& ParseDocumentParams(rapidjson::Document& document) const;

	const bool IsErrorField(const std::string& jsonString) const;
	const bool haveTokenField(const std::string& jsonString) const;
	const bool ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const;

	const std::string CreateJsonString(const std::unordered_map<std::string, std::string>& args, const std::unordered_map<std::string, std::string>& params) const;
	const std::string ParseSessionToken(const std::string& jsonString) const;

private:
	JsonWrapper();
};

#endif // !JSON_WRAPPER_H
