#include "api.h"

#include <iostream>
#include <boost/format.hpp>

#include "../../secure/crypt/crypt.h"
#include "../../logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
	CREATE_EVENT_LOG("API Interface created\n")
	this->getUserData(jsonString);
}

bool API::isAuthorized() {
	return checkUserAuthentication() && checkUserHwid() && checkUserLicense() && checkUserToken();
}

void API::getUserData(const std::string& jsonString) {
	if (jsonString.empty()) {
		CREATE_EVENT_LOG("Failed to get user data (empty argument)")
		return;
	}

	this->data = JsonWrapper::getInstance()->parseUserData(jsonString);
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

	CREATE_EVENT_LOG("Request to verify account availability")
	return this->performApiRequest(jsonString);
}

bool API::checkUserHwid() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "hwid" },
		  { "type", "update" }
		},
		{
			{ "member_id", this->data.member_id },
			{ "hwid", this->data.hwid }
		}
		);

	CREATE_EVENT_LOG("Request to verify HWID")
	return this->performApiRequest(jsonString);
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

	CREATE_EVENT_LOG("Request to verify the availability license")
	return this->performApiRequest(jsonString);
}

bool API::checkUserToken() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "session" },
		  { "type", "validate" }
		},
		{
			{ "username", this->data.username },
			{ "hwid", this->data.hwid },
			{ "token", this->data.token }
		}
		);

	CREATE_EVENT_LOG("Request to verify session token")
	return this->performApiRequest(jsonString);
}

bool API::performApiRequest(const std::string& jsonString) {
	std::string encryptedJson = Crypt::encryptBase64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % this->url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = Crypt::decryptBase64(response);

	if (JsonWrapper::getInstance()->haveMemberIdField(decryptedResponse)) {
		this->data.member_id = JsonWrapper::getInstance()->parseMemberId(decryptedResponse);
	}

	if (JsonWrapper::getInstance()->haveErrorField(decryptedResponse)) {
		CREATE_EVENT_LOG("Request status: failed\n")
		return false;
	}

	CREATE_EVENT_LOG("Request status: success\n")

	return true;
}
