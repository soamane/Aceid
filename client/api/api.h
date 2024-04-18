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
	std::string member_id;
};

class API
{
public:
	API(const AuthData& data);

	const std::string convertAuthDataToJson();

private:
	AuthData data;
	const std::string url = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
