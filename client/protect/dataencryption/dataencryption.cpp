#include "dataencryption.h"

#include "base64/base64.h"

std::string DataEncryption::encryptBase64(const std::string& str) {
	return base64::to_base64(str);
}

std::string DataEncryption::decryptBase64(const std::string& str) {
	return base64::from_base64(str);
}
