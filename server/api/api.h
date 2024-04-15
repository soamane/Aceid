#pragma once
#ifndef API_H
#define API_H

#include <string>

#include "../jsonparser/jsonparser.h"

class Api 
{
public:
	Api(const AuthData& authData);

private:
	AuthData authData;
	const std::string source = "https://aceid.cc/server_api/api.php";
};

#endif // !API_H
