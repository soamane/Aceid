#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

class DataEncryption {
public:
	static const std::string EncryptBase64(const std::string& source);
	static const std::string DecryptBase64(const std::string& source);

	static const std::string EncryptMultiBase64(const std::string& source);
	static const std::string DecryptMultiBase64(const std::string& source);

	static const std::string EncryptCustomMethod(const std::string& source);
	static const std::string DecryptCustomMethod(const std::string& source);

	static const std::vector<char> EncryptBuffer(const std::vector<char>& source);
	static const std::vector<char> DecryptBuffer(const std::vector<char>& source);

private:
	static const int GenerateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = "aceid"; // encode salt
};

#endif // !DATA_ENCRYPTION_H
