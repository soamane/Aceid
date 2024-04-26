#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

class DataEncryption
{
public:
	static std::string encryptBase64(const std::string& str);
	static std::string decryptBase64(const std::string& str);

	static std::string encryptMultiBase64(const std::string& str);
	static std::string decryptMultiBase64(const std::string& str);

	static std::string encryptCustomMethod(const std::string& str);
	static std::string decryptCustomMethod(const std::string& str);

private:
	static int generateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string salt = "wtfdude?";
};

#endif // !DATA_ENCRYPTION_H
