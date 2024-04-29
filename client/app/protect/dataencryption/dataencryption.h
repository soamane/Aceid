#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

class DataEncryption
{
public:
	static const std::string encryptBase64(const std::string& str);
	static const std::string decryptBase64(const std::string& str);

	static const std::string encryptMultiBase64(const std::string& str);
	static const std::string decryptMultiBase64(const std::string& str);

	static const std::string encryptCustomMethod(const std::string& str);
	static const std::string decryptCustomMethod(const std::string& str);

private:
	static const int generateTimeStamp();
	static const int generateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = "aceid";
};

#endif // !DATA_ENCRYPTION_H
