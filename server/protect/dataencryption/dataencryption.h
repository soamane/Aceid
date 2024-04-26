#pragma once
#ifndef DATA_ENCRYPTION_H
#define DATA_ENCRYPTION_H

#include <string>

/*

	Class for working with encryption and so on

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

private:
	static inline const std::string salt = "wtfdude?"; // encode salt
};

#endif // !DATA_ENCRYPTION_H
