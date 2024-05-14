#include "dataencryption.h"

#include <iomanip>
#include <sstream>

#include "base64/base64.h"

const std::string DataEncryption::encryptBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	return base64::to_base64(source);
}

const std::string DataEncryption::decryptBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	return base64::from_base64(source);
}

const std::string DataEncryption::encryptMultiBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::string result;
	for (auto& c : source) {
		result += encryptBase64(std::string(1, c));
	}

	std::size_t pos = result.find('=');
	while (pos != std::string::npos) {
		result.replace(pos, 1, key);
		pos = result.find('=', pos + 1);
	}

	result = encryptBase64(result);
	return result;
}

const std::string DataEncryption::decryptMultiBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::string decrypted = decryptBase64(source);
	if (decrypted.empty()) {
		throw std::runtime_error(xorstr_("Failed to decrypt the source"));
	}

	std::size_t pos = decrypted.find(key);
	while (pos != std::string::npos) {
		decrypted.replace(pos, key.size(), "=");
		pos = decrypted.find(key, pos + 1);
	}

	std::string result;
	const std::size_t offset = 4;
	for (std::size_t pad = 0; pad < decrypted.length(); pad += offset) {
		std::string chunk = decrypted.substr(pad, offset);
		result += decryptBase64(chunk);
	}

	return result;
}

const std::string DataEncryption::encryptCustomMethod(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::vector<int> sourceData(source.begin(), source.end());
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = generateKeyCode(keyData);
	for (auto& it : sourceData) {
		it -= keyCode;
	}

	std::string result = std::string(sourceData.begin(), sourceData.end());
	if (result.empty()) {
		throw std::runtime_error(xorstr_("Failed to initialize result string"));
	}

	result = encryptMultiBase64(result);

	return result;
}

const std::string DataEncryption::decryptCustomMethod(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	const std::string decrypted = decryptMultiBase64(source);
	if (decrypted.empty()) {
		throw std::runtime_error(xorstr_("Failed to decrypt the source"));
	}

	std::vector<int> sourceData(decrypted.begin(), decrypted.end());
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = generateKeyCode(keyData);
	for (auto& it : sourceData) {
		it += keyCode;
	}

	const std::string result = std::string(sourceData.begin(), sourceData.end());
	return result;
}

const std::vector<char> DataEncryption::encryptBuffer(const std::vector<char>& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source data)"));
	}

	std::vector<char> encryptedData(source);
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = generateKeyCode(keyData);
	for (char& ch : encryptedData) {
		ch -= static_cast<char>(keyCode);
	}

	return encryptedData;
}

const std::vector<char> DataEncryption::decryptBuffer(const std::vector<char>& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source data)"));
	}

	std::vector<char> encryptedData(source);
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = generateKeyCode(keyData);
	for (char& ch : encryptedData) {
		ch += static_cast<char>(keyCode);
	}

	return encryptedData;
}

const int DataEncryption::generateKeyCode(const std::vector<int>& keyData) {
	if (keyData.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (key data)"));
	}

	std::vector<int> encryptedKeyData = keyData;
	for (std::size_t i = 0; i < encryptedKeyData.size() / 2; ++i) {
		std::swap(encryptedKeyData[i], encryptedKeyData[encryptedKeyData.size() - i - 1]);
	}

	int result = 0;
	for (std::size_t i = 0; i < encryptedKeyData.size(); ++i) {
		if (i % 2 == 0) {
			result += encryptedKeyData[i] >> 3;
		} else {
			result -= encryptedKeyData[i] << 3;
		}
	}

	return result;
}
