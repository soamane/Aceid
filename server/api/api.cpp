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

bool API::checkUserAuthentication() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", this->data.username },
			{ "password", this->data.password }
		}
	);

	return this->performCheckCredentials(jsonString);
}

bool API::checkUserHwid() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" },
		  { "type", "hwid" }
		},
		{
			{ "member_id", this->data.member_id },
			{ "hwid", this->data.hwid }
		}
		);

	return this->performCheckCredentials(jsonString);
}

bool API::checkUserLicense() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "license" },
		  { "type", "get" }
		},
		{
			{ "username", this->data.username },
			{ "password", this->data.password }
		}
	);

	return this->performCheckCredentials(jsonString);
}

bool API::performCheckCredentials(const std::string& jsonString) {
	boost::format formatString = boost::format("%1%data=%2%") % this->source % jsonString;
	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, formatString.str(), nullptr);

	return JsonWrapper::getInstance()->isErrorField(response);
}
