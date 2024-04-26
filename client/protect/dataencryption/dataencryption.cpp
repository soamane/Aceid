#include "dataencryption.h"

#include "base64/base64.h"

std::string DataEncryption::encryptBase64(const std::string& str) {
	return base64::to_base64(str);
}

std::string DataEncryption::decryptBase64(const std::string& str) {
	return base64::from_base64(str);
}

std::string DataEncryption::encryptMultiBase64(const std::string& str) {
	std::string result;
	for (auto& c : str) {
		std::string encryptedChar = encryptBase64(std::string(1, c));
		result += encryptedChar;
	}

	size_t pos = result.find('=');
	while (pos != std::string::npos) {
		result.replace(pos, 1, salt);
		pos = result.find('=', pos + 1);
	}

	result = encryptBase64(result);
	return result;
}

std::string DataEncryption::decryptMultiBase64(const std::string& str) {
	std::string result;
	std::string decryptedStr = decryptBase64(str);

	size_t pos = decryptedStr.find(salt);
	while (pos != std::string::npos) {
		decryptedStr.replace(pos, salt.size(), "=");
		pos = decryptedStr.find(salt, pos + 1);
	}

	for (size_t i = 0; i < decryptedStr.length(); i += 4) {
		std::string chunk = decryptedStr.substr(i, 4);
		result += base64::from_base64(chunk);
	}

	return result;
}

std::string DataEncryption::encryptCustomMethod(const std::string& str) {
	std::vector<int> strData(str.begin(), str.end());
	std::vector<int> keyData(salt.begin(), salt.end());

	int keycode = generateKeyCode(keyData);
	for (auto& c : strData) {
		c -= keycode;
	}

	return std::string(strData.begin(), strData.end());
}

std::string DataEncryption::decryptCustomMethod(const std::string& str) {
	std::vector<int> strData(str.begin(), str.end());
	std::vector<int> keyData(salt.begin(), salt.end());

	int keycode = generateKeyCode(keyData);
	for (auto& c : strData) {
		c += keycode;
	}

	return std::string(strData.begin(), strData.end());
}

int DataEncryption::generateKeyCode(const std::vector<int>& keyData) {
	int keycode = 0;
	int halfSize = keyData.size() / 2;

	for (std::size_t i = 0; i < keyData.size(); ++i) {
		if (keyData.size() % 2 == 0) {
			if (i >= halfSize) {
				keycode -= keyData[i];
			}
			else {
				keycode += keyData[i];
			}
		}
		else {
			if (i == keyData.size() - 1) {
				keycode -= keyData[i];
			}
			else {
				keycode += keyData[i];
			}
		}
	}

	keycode *= -1 * static_cast<int>(keyData.size());
	return keycode;
}

