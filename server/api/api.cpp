#include "api.h"

void API::getClientAuthData(const std::string& jsonString) {
	if (jsonString.empty()) {
		// TODO: error log
		return;
	}
	this->data = JsonWrapper::getInstance()->parseAuthData(jsonString);
}
