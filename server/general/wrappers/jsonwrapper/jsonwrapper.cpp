#include "jsonwrapper.h"

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <stdexcept>

#include "../../../general/logsystem/logmanager/logmanager.h"

JsonWrapper* JsonWrapper::getInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

const rapidjson::Document JsonWrapper::parseJsonString(const std::string& jsonString) {
	rapidjson::Document document;
	document.Parse(jsonString.c_str());

	if (!document.IsObject()) {
		CREATE_EVENT_LOG("failed parse json document");
		return nullptr;
	}

	return document;
}

const rapidjson::Value& JsonWrapper::parseDocumentParams(rapidjson::Document& document) {
	const rapidjson::Value& params = document["params"];
	if (!params.IsObject()) {
		throw std::runtime_error("document params is not of object");
	}

	return params;
}

const std::string JsonWrapper::parseMemberId(const std::string& jsonString) {
	auto document = parseJsonString(jsonString);
	if (!document.IsObject()) {
		return std::string();
	}

	auto& params = parseDocumentParams(document);
	if (!params.IsObject()) {
		return std::string();
	}

	return std::to_string(params["id"].GetInt());
}

const AuthData JsonWrapper::parseUserData(const std::string& jsonString) {
	auto document = parseJsonString(jsonString);
	if (!document.IsObject()) {
		return AuthData();
	}

	auto& params = parseDocumentParams(document);
	if (!params.IsObject()) {
		return AuthData();
	}

	AuthData authData;
	authData.username = params["username"].GetString();
	authData.password = params["password"].GetString();
	authData.hwid = params["hwid"].GetString();
	authData.token = params["token"].GetString();

	return authData;
}

bool JsonWrapper::haveErrorField(const std::string& jsonString) {
	return paramsFieldExist(jsonString, "error");
}

bool JsonWrapper::haveMemberIdField(const std::string& jsonString) {
	return paramsFieldExist(jsonString, "id");
}

bool JsonWrapper::paramsFieldExist(const std::string& jsonString, const std::string& fieldName) {
	auto document = parseJsonString(jsonString);
	if (!document.IsObject()) {
		return false;
	}

	auto& params = parseDocumentParams(document);
	if (!params.IsObject()) {
		return false;
	}

	return params.HasMember(fieldName.c_str());
}

const std::string JsonWrapper::createJsonString(std::initializer_list<std::pair<std::string, std::string>> fields, std::initializer_list<std::pair<std::string, std::string>> args) {
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType allocator;

	for (const auto& add : fields) {
		rapidjson::Value key(add.first.data(), allocator);
		rapidjson::Value value(add.second.data(), allocator);
		document.AddMember(key, value, allocator);
	}

	rapidjson::Value params(rapidjson::kObjectType);
	for (const auto& arg : args) {
		rapidjson::Value key(arg.first.data(), allocator);
		rapidjson::Value value(arg.second.data(), allocator);
		params.AddMember(key, value, allocator);
	}
	document.AddMember("params", params, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return buffer.GetString();
}


JsonWrapper::JsonWrapper() {
}
