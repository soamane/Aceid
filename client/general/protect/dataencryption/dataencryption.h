#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

#include "../xorstring/xorstring.h"

class DataEncryption {
public:
	static const std::string EncryptBase64(const std::string& str);
	static const std::string DecryptBase64(const std::string& str);

	static const std::string EncryptMultiBase64(const std::string& str);
	static const std::string DecryptMultiBase64(const std::string& str);

	static const std::string EncryptCustomMethod(const std::string& str);
	static const std::string DecryptCustomMethod(const std::string& str);

	static const std::vector<char> EncryptBuffer(const std::vector<char>& source);
	static const std::vector<char> DecryptBuffer(const std::vector<char>& source);

private:
	static const int GenerateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = xorstr_("aceid");
};

#endif // !DATA_ENCRYPTION_H
