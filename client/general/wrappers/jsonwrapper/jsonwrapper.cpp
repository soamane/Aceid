#include "jsonwrapper.h"

#include <stdexcept>

#include "../../protect/xorstring/xorstring.h"

JsonWrapper* JsonWrapper::GetInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

const rapidjson::Document JsonWrapper::ConvertStringToJson(const std::string& jsonString) const {
	rapidjson::Document document;
	document.Parse(jsonString.c_str());

	if (!document.IsObject()) {
		throw std::runtime_error(xorstr_("Failed to parse json document"));
	}

	return document;
}

const rapidjson::Value& JsonWrapper::ParseDocumentParams(rapidjson::Document& document) const {
	const rapidjson::Value& params = document[xorstr_("params")];

	if (!params.IsObject()) {
		throw std::runtime_error(xorstr_("'params' field is not of object"));
	}

	return params;
}

const bool JsonWrapper::IsErrorField(const std::string& jsonString) const {
	return ParamsFieldExist(jsonString, xorstr_("error"));
}

const bool JsonWrapper::haveTokenField(const std::string& jsonString) const {
	return ParamsFieldExist(jsonString, xorstr_("token"));
}

const bool JsonWrapper::ParamsFieldExist(const std::string& jsonString, const std::string& fieldName) const {
	auto document = ConvertStringToJson(jsonString);
	auto& params = ParseDocumentParams(document);

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


const std::string JsonWrapper::ParseSessionToken(const std::string& jsonString) const {
	auto document = ConvertStringToJson(jsonString);
	auto& params = ParseDocumentParams(document);

	if (!params.HasMember(xorstr_("token"))) {
		throw std::runtime_error(xorstr_("Failed to parse token field"));
	}

	return params[xorstr_("token")].GetString();
}

JsonWrapper::JsonWrapper() {

}