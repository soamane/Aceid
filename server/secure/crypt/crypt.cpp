#include "crypt.h"

#include "base64/base64.h"

std::string Crypt::encryptBase64(const std::string& str) {
	return base64::to_base64(str);
}

std::string Crypt::decryptBase64(const std::string& str) {
	return base64::from_base64(str);
}
