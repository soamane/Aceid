#pragma once
#ifndef API_H
#define API_H

#include <memory>
#include <string>

#include "../../general/wrappers/jsonwrapper/jsonwrapper.h"
#include "../../general/wrappers/curlwrapper/curlwrapper.h"

struct AuthData
{
	std::string username;
	std::string password;
	std::string hwid;

	std::string token;
};

class API
{
public:
	API(AuthData* data);

	const std::string convertAuthDataToJson();
	const std::string getSessionToken();

private:
	const std::string performGetSessionToken(const std::string& jsonString);

private:
	AuthData* m_authData;
	const std::string m_url = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
