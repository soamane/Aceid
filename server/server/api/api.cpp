#include "api.h"

#include <boost/format.hpp>
#include "../../extern/base64/base64.h"

API::API(const std::string& jsonString) {
	this->getUserData(jsonString);
}

bool API::isAuthorized() {
	return checkUserAuthentication() && checkUserHwid() && checkUserLicense() && checkUserToken();
}

void API::getUserData(const std::string& jsonString) {
	if (jsonString.empty()) {
		// TODO: error log
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

	return this->performApiRequest(jsonString);
}

bool API::performApiRequest(const std::string& jsonString) {
	std::string encryptedJson = base64::to_base64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % this->url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = base64::from_base64(response);

	if (JsonWrapper::getInstance()->haveMemberIdField(decryptedResponse)) {
		this->data.member_id = JsonWrapper::getInstance()->parseMemberId(decryptedResponse);
	}

	if (JsonWrapper::getInstance()->haveErrorField(decryptedResponse)) {
		return false;
	}

	return true;
}
