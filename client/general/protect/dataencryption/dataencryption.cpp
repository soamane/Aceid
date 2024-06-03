#include "dataencryption.h"

#include <iomanip>
#include <sstream>

#include "base64/base64.h"

const std::string DataEncryption::EncryptBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	return base64::to_base64(source);
}

const std::string DataEncryption::DecryptBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	return base64::from_base64(source);
}

const std::string DataEncryption::EncryptMultiBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::string result;
	for (auto& c : source) {
		result += EncryptBase64(std::string(1, c));
	}

	std::size_t pos = result.find('=');
	while (pos != std::string::npos) {
		result.replace(pos, 1, key);
		pos = result.find('=', pos + 1);
	}

	result = EncryptBase64(result);
	return result;
}

const std::string DataEncryption::DecryptMultiBase64(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::string decrypted = DecryptBase64(source);
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
		result += DecryptBase64(chunk);
	}

	return result;
}

const std::string DataEncryption::EncryptCustomMethod(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	std::vector<int> sourceData(source.begin(), source.end());
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = GenerateKeyCode(keyData);
	for (auto& it : sourceData) {
		it -= keyCode ^ key.size();
	}

	std::string result = std::string(sourceData.begin(), sourceData.end());
	if (result.empty()) {
		throw std::runtime_error(xorstr_("Failed to initialize result string"));
	}

	result = EncryptMultiBase64(result);

	return result;
}

const std::string DataEncryption::DecryptCustomMethod(const std::string& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source)"));
	}

	const std::string decrypted = DecryptMultiBase64(source);
	if (decrypted.empty()) {
		throw std::runtime_error(xorstr_("Failed to decrypt the source"));
	}

	std::vector<int> sourceData(decrypted.begin(), decrypted.end());
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = GenerateKeyCode(keyData);
	for (auto& it : sourceData) {
		it += keyCode ^ key.size();
	}

	const std::string result = std::string(sourceData.begin(), sourceData.end());
	return result;
}

const std::vector<char> DataEncryption::EncryptBuffer(const std::vector<char>& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source data)"));
	}

	std::vector<char> encryptedData(source);
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = GenerateKeyCode(keyData);
	for (char& ch : encryptedData) {
		ch -= static_cast<char>(keyCode);
	}

	return encryptedData;
}

const std::vector<char> DataEncryption::DecryptBuffer(const std::vector<char>& source) {
	if (source.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (source data)"));
	}

	std::vector<char> encryptedData(source);
	std::vector<int> keyData(key.begin(), key.end());

	const int keyCode = GenerateKeyCode(keyData);
	for (char& ch : encryptedData) {
		ch += static_cast<char>(keyCode);
	}

	return encryptedData;
}

const int DataEncryption::GenerateKeyCode(const std::vector<int>& keyData) {
	if (keyData.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (key data)"));
	}

	std::vector<int> encryptedKeyData = keyData;
	const std::size_t middle = encryptedKeyData.size() / 2;

	for (std::size_t i = 0; i < middle; ++i) {
		std::swap(encryptedKeyData[i], encryptedKeyData[encryptedKeyData.size() - i - 1]);
	}

	std::reverse(encryptedKeyData.begin() + middle, encryptedKeyData.end());

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