#pragma once
#ifndef CRYPT_H
#define CRYPT_H

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

private:

};

#endif // !CRYPT_H
