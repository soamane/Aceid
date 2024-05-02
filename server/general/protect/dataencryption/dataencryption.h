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
	static const std::string encryptBase64(const std::string& source);

	// Decodes a string using base64
	static const std::string decryptBase64(const std::string& source);

	// Encodes a string using custom method based on base64
	static const std::string encryptMultiBase64(const std::string& source);

	// Decodes a string using custom method based on base64
	static const std::string decryptMultiBase64(const std::string& source);

	// Encodes a string using custom method
	static const std::string encryptCustomMethod(const std::string& source);

	// Decodes a string using custom method
	static const std::string decryptCustomMethod(const std::string& source);

	// Encodes a char buffer using custom encryption method
	static const std::vector<char> encryptBuffer(const std::vector<char>& source);

	// Decodes a char buffer using custom decryption method
	static const std::vector<char> decryptBuffer(const std::vector<char>& source);

private:
	// Get unique runtime time stamp
	static const int generateTimeStamp( );

	// Get keycode via key word
	static const int generateKeyCode(const std::vector<int>& keyData);

private:
	static inline const std::string key = "aceid"; // encode salt
};

#endif // !DATA_ENCRYPTION_H
