#pragma once
#ifndef API_H
#define API_H

#include <string>

struct AuthData
{
	std::string username;
	std::string password;
	std::string hwid;
};

class Api 
{
public:
	Api(const AuthData& authData);

private:
	AuthData authData;

};

#endif // !API_H
