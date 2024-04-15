#include "json.h"

Json* Json::getInstance() {
	static Json* Instance = new Json();
	return Instance;
}

const AuthData Json::parseAuthData(const std::string& jsonString) {
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

Json::Json() {
}
