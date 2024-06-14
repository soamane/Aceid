#include "jsonwrapper.h"

#include <stdexcept>

#include "../../protect/xorstring/xorstring.h"

JsonWrapper* JsonWrapper::GetInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

const rapidjson::Document JsonWrapper::ConvertStringToJson(std::string_view jsonString) const {
	rapidjson::Document document;
	document.Parse(jsonString.data());

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

const bool JsonWrapper::IsErrorField(std::string_view jsonString) const {
	return ParamsFieldExist(jsonString, xorstr_("error"));
}

const bool JsonWrapper::haveTokenField(std::string_view jsonString) const {
	return ParamsFieldExist(jsonString, xorstr_("token"));
}

const bool JsonWrapper::ParamsFieldExist(std::string_view jsonString, std::string_view fieldName) const {
	auto document = ConvertStringToJson(jsonString);
	auto& params = ParseDocumentParams(document);

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


const std::string JsonWrapper::ParseSessionToken(std::string_view jsonString) const {
	auto document = ConvertStringToJson(jsonString);
	auto& params = ParseDocumentParams(document);

	if (!haveTokenField(jsonString)) {
		throw std::runtime_error(xorstr_("Failed to parse token field"));
	}

	return params[xorstr_("token")].GetString();
}

JsonWrapper::JsonWrapper() {

}