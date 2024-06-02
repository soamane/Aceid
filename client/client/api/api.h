#pragma once
#ifndef API_H
#define API_H

#include <memory>
#include <string>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

#include "../../general/protect/xorstring/xorstring.h"

struct AuthData {
	std::string username;
	std::string password;
	std::string hwid;

	std::string token;
	std::string launchParams;
};

class API {
public:
	API(AuthData* data);

	const std::string ConvertAuthDataToJson();
	const std::string CreateLaunchParams();
	const std::string GetSessionToken();

private:
	const std::string PerformRequestToGetSessionToken(const std::string& jsonString);

private:
	AuthData* m_authData;
	const std::string m_url = xorstr_("https://aceid.cc/server_api/api.php");
};

#endif // !API_H
