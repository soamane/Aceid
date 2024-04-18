#pragma once
#ifndef API_H
#define API_H

#include <string>

#include "../wrappers/jsonwrapper/jsonwrapper.h"
#include "../wrappers/curlwrapper/curlwrapper.h"

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
	API(const AuthData& data);

	const std::string convertAuthDataToJson();
	const std::string getSessionToken();
private:
	const std::string performGetSessionToken(const std::string& jsonString);

private:
	AuthData data;
	const std::string url = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
