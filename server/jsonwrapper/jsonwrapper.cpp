#include "jsonwrapper.h"

JsonWrapper* JsonWrapper::getInstance() {
	static JsonWrapper* Instance = new JsonWrapper();
	return Instance;
}

const AuthData JsonWrapper::parseAuthData(const std::string& jsonString) {
	rapidjson::Document document;
	document.Parse(jsonString.c_str());
	if (!document.IsObject()) {
		// TODO: error log
		return AuthData();
	}

	const rapidjson::Value& params = document["params"];
	if (!params.IsObject()) {
		// TODO: error log
		return AuthData();
	}

	AuthData authData;
	authData.username = params["username"].GetString();
	authData.password = params["password"].GetString();
	authData.hwid = params["hwid"].GetString();

	return authData;
}

bool JsonWrapper::isErrorField(const std::string& jsonString) {
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

	return params.HasMember("error");
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
