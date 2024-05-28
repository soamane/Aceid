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

const std::string JsonWrapper::CreateJsonString(std::initializer_list<std::pair<std::string, std::string>> additionals, std::initializer_list<std::pair<std::string, std::string>> args) const {
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType allocator;

	for (const auto& add : additionals) {
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