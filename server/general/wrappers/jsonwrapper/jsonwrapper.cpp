#include "jsonwrapper.h"

#include <stdexcept>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../../../general/logsystem/logmanager/logmanager.h"

JsonWrapper* JsonWrapper::GetInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	if (!Instance) {
		throw std::runtime_error("Failed to initialize JSON wrapper");
	}

	return Instance;
}

const rapidjson::Document JsonWrapper::ConvertStringToJson(const std::string& jsonString) const {
	if (jsonString.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	rapidjson::Document document;
	document.Parse(jsonString.c_str());

	if (!document.IsObject()) { 
		throw std::runtime_error("Failed parse json document");
	}

	return document;
}

const rapidjson::Value& JsonWrapper::ParseDocumentParams(rapidjson::Document& document) const {
	if (!document.IsObject()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	const rapidjson::Value& params = document["params"];
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	return params;
}

const std::string JsonWrapper::ParseParamsField(const std::string& jsonString, const std::string& fieldName) const {
	if (jsonString.empty() || fieldName.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	auto document = ConvertStringToJson(jsonString);
	if (!document.IsObject()) {
		throw std::runtime_error("Failed parse json document");
	}

	auto& params = ParseDocumentParams(document);
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	const rapidjson::Value& fieldValue = params[fieldName.c_str()];
	switch (fieldValue.GetType()) {
		case rapidjson::kStringType:
			return fieldValue.GetString();

		case rapidjson::kNumberType:
			return std::to_string(fieldValue.GetInt());
	}
}

const AuthData JsonWrapper::ParseUserData(const std::string& jsonString) const {
	if (jsonString.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	auto document = ConvertStringToJson(jsonString);
	if (!document.IsObject()) {
		throw std::runtime_error("Failed parse json document");
	}

	auto& params = ParseDocumentParams(document);
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	AuthData authData;
	authData.username = params["username"].GetString();
	authData.password = params["password"].GetString();
	authData.hwid = params["hwid"].GetString();
	authData.token = params["token"].GetString();

	return authData;
}

const bool JsonWrapper::IsErrorField(const std::string& jsonString) const {
	return ParamsFieldExist(jsonString, "error");
}

const bool JsonWrapper::IsMemberIdField(const std::string& jsonString) const {
	return ParamsFieldExist(jsonString, "id");
}

const bool JsonWrapper::ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const {
	auto document = ConvertStringToJson(jsonString);
	if (!document.IsObject()) {
		throw std::runtime_error("Failed parse json document");
	}

	auto& params = ParseDocumentParams(document);
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	return params.HasMember(fieldName.c_str());
}

const std::string JsonWrapper::CreateJsonString(const std::unordered_map<std::string, std::string>& additionals, const std::unordered_map<std::string, std::string>& params) const {
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	for (const auto& add : additionals) {
		rapidjson::Value key(add.first.c_str(), allocator);
		rapidjson::Value value(add.second.c_str(), allocator);
		document.AddMember(key, value, allocator);
	}

	rapidjson::Value paramsObject(rapidjson::kObjectType);
	for (const auto& param : params) {
		rapidjson::Value key(param.first.c_str(), allocator);
		rapidjson::Value value(param.second.c_str(), allocator);
		paramsObject.AddMember(key, value, allocator);
	}
	document.AddMember("params", paramsObject, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return buffer.GetString();
}



JsonWrapper::JsonWrapper() { }
