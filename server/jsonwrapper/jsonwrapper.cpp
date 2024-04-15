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

JsonWrapper::JsonWrapper() {
}
