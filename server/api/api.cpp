#include "api.h"

#include <boost/format.hpp>

#include <iostream>

API::API(const std::string& jsonString) {
	this->getClientAuthData(jsonString);
}

void API::getClientAuthData(const std::string& jsonString) {
	if (jsonString.empty()) {
		// TODO: error log
		return;
	}
	this->data = JsonWrapper::getInstance()->parseAuthData(jsonString);
}

bool API::isAuthorized() {
	static JsonWrapper* jsonWrapper = JsonWrapper::getInstance();
	if (jsonWrapper == nullptr) {
		// TODO: exception log
		return false;
	}

	const std::string jsonString = jsonWrapper->createJsonString
	(
		{ {"action", "auth"} },
		{
			{ "username", this->data.username },
			{ "password", this->data.password }
		}
	);

	static CurlWrapper* curlWrapper = CurlWrapper::getInstance();
	if (curlWrapper == nullptr) {
		// TODO: exception log
		return false;
	}

	boost::format formatString = boost::format("%1%data=%2%") % this->source % jsonString;
	const std::string response = curlWrapper->performRequest(RequestType::eRT_HTTPS, formatString.str(), nullptr);

	return !jsonWrapper->isErrorField(response);
}
