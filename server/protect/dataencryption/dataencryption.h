#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>
#include <vector>

/*

	Class for working with encryption

*/

class DataEncryption
{
public:
	// Encodes a string using base64
	static std::string encryptBase64(const std::string& str);

	// Decodes a string using base64
	static std::string decryptBase64(const std::string& str);

	// Encodes a string using custom method based on base64
	static std::string encryptMultiBase64(const std::string& str);

	// Decodes a string using custom method based on base64
	static std::string decryptMultiBase64(const std::string& str);

	// Encodes a string using custom method
	static std::string encryptCustomMethod(const std::string& str);

	// Decodes a string using custom method
	static std::string decryptCustomMethod(const std::string& str);

private:

	// Get keycode via key word
	static int generateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = "aceidnetwork"; // encode salt
};

#endif // !DATA_ENCRYPTION_H
