#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>

class DataEncryption
{
public:
	static std::string encryptBase64(const std::string& str);
	static std::string decryptBase64(const std::string& str);

private:

};

#endif // !DATA_ENCRYPTION_H
