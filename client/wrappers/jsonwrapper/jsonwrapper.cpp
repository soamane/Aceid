#include "jsonwrapper.h"

JsonWrapper* JsonWrapper::getInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

bool JsonWrapper::haveErrorField(const std::string& jsonString) {
	return this->paramsFieldExist(jsonString, "error");
}

bool JsonWrapper::paramsFieldExist(const std::string& jsonString, const std::string& fieldName) {
	rapidjson::Document document;
	document.Parse(jsonString.c_str());
	if (!document.IsObject()) {
		// TODO: error log
		return false;
	}

	const rapidjson::Value& params = document["params"];
	if (!params.IsObject()) {
		// TODO: error log
		return false;
	}

	return params.HasMember(fieldName.c_str());
}

const std::string JsonWrapper::createJsonString(std::initializer_list<std::pair<std::string, std::string>> additionals, std::initializer_list<std::pair<std::string, std::string>> args) {
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

JsonWrapper::JsonWrapper() {

}