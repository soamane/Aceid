#include "api.h"

#include <iostream>
#include <boost/format.hpp>

#include "../../secure/crypt/crypt.h"
#include "../../logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
	CREATE_EVENT_LOG("API Interface created\n")
	getUserData(jsonString);
}

bool API::isAuthorized() {
	return checkUserAuthentication() && checkUserHwid() && checkUserLicense() && checkUserToken();
}

void API::getUserData(const std::string& jsonString) {
	if (jsonString.empty()) {
		CREATE_EVENT_LOG("Failed to get user data (empty argument)")
		return;
	}

	data = JsonWrapper::getInstance()->parseUserData(jsonString);
}

bool API::checkUserAuthentication() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", data.username },
			{ "password", data.password }
		}
		);

	CREATE_EVENT_LOG("Request to verify account availability")
	return performApiRequest(jsonString);
}

bool API::checkUserHwid() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "hwid" },
		  { "type", "update" }
		},
		{
			{ "member_id", data.member_id },
			{ "hwid", data.hwid }
		}
		);

	CREATE_EVENT_LOG("Request to verify HWID")
	return performApiRequest(jsonString);
}

bool API::checkUserLicense() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "license" },
		  { "type", "get" }
		},
		{
			{ "username", data.username },
			{ "password", data.password }
		}
		);

	CREATE_EVENT_LOG("Request to verify the availability license")
	return performApiRequest(jsonString);
}

bool API::checkUserToken() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "session" },
		  { "type", "validate" }
		},
		{
			{ "username", data.username },
			{ "hwid", data.hwid },
			{ "token", data.token }
		}
		);

	CREATE_EVENT_LOG("Request to verify session token")
	return performApiRequest(jsonString);
}

bool API::performApiRequest(const std::string& jsonString) {
	std::string encryptedJson = Crypt::encryptBase64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = Crypt::decryptBase64(response);

	if (JsonWrapper::getInstance()->haveMemberIdField(decryptedResponse)) {
		data.member_id = JsonWrapper::getInstance()->parseMemberId(decryptedResponse);
	}

	if (JsonWrapper::getInstance()->haveErrorField(decryptedResponse)) {
		CREATE_EVENT_LOG("Request status: failed\n")
		return false;
	}

	CREATE_EVENT_LOG("Request status: success\n")

	return true;
}

const std::string API::getUsername() {
	return data.username;
}
