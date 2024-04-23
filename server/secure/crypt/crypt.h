#pragma once
#ifndef CRYPT_H
#define CRYPT_H

#include <string>

class Crypt
{
public:
	static std::string encryptBase64(const std::string& str);
	static std::string decryptBase64(const std::string& str);

private:

};

#endif // !CRYPT_H
