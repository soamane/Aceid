#include "jsonwrapper.h"

#include <stdexcept>
#include <boost/format.hpp>
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

const rapidjson::Document JsonWrapper::ConvertStringToJson(std::string_view jsonString) const {
	rapidjson::Document document;
	document.Parse(jsonString.data());

	if (!document.IsObject()) { 
		throw std::runtime_error("Failed parse json document");
	}

	return document;
}

const rapidjson::Value& JsonWrapper::ParseDocumentParams(rapidjson::Document& document) const {
	const rapidjson::Value& params = document["params"];
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	return params;
}

const std::string JsonWrapper::ParseParamsField(std::string_view jsonString, std::string_view fieldName) const {
	auto document = ConvertStringToJson(jsonString);
	if (!document.IsObject()) {
		throw std::runtime_error("Failed parse json document");
	}

	auto& params = ParseDocumentParams(document);
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	const rapidjson::Value& fieldValue = params[fieldName.data()];
	switch (fieldValue.GetType()) {
		case rapidjson::kStringType:
			return fieldValue.GetString();

		case rapidjson::kNumberType:
			return std::to_string(fieldValue.GetInt());
	}
}

const AuthData JsonWrapper::ParseUserData(std::string_view jsonString) const {
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

const bool JsonWrapper::IsErrorField(std::string_view jsonString) const {
	return ParamsFieldExist(jsonString, "error");
}

const bool JsonWrapper::IsMemberIdField(std::string_view jsonString) const {
	return ParamsFieldExist(jsonString, "id");
}

const bool JsonWrapper::ParamsFieldExist(std::string_view jsonString, std::string_view fieldName) const {
	auto document = ConvertStringToJson(jsonString);
	if (!document.IsObject()) {
		throw std::runtime_error("Failed parse json document");
	}

	auto& params = ParseDocumentParams(document);
	if (!params.IsObject()) {
		throw std::runtime_error("Document params is not of object");
	}

	return params.HasMember(fieldName.data());
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
