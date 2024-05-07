#include "api.h"

#include <boost/format.hpp>

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::runtime_error("Function call error: empty argument (json string)");
	}

	CREATE_EVENT_LOG("API initialized successfully");
	getUserData(jsonString);
}

bool API::isAuthorized() {
	return checkUserAuthentication() && checkUserHwid() && 
		checkUserLicense() && checkUserToken();
}

void API::getUserData(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::runtime_error("Function call error: empty argument (JSON string)");
	}

	m_authData = JsonWrapper::getInstance()->parseUserData(jsonString);
}

void API::getProfileGroup() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "groups" },
		  { "type", "get" } },
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password },
			{ "hwid", m_authData.hwid }
		}
		);

	CREATE_EVENT_LOG("Request to get profile group");

	std::optional<const std::string> requestResult = performApiRequest(jsonString);
	if (!requestResult.has_value()) {
		CREATE_EVENT_LOG("Failed to get 'groups' field value");
		return;
	}

	const std::string response = requestResult.value();
	if (response.empty()) {
		CREATE_EVENT_LOG("The response result is empty");
		return;
	}

	m_authData.profile_group = JsonWrapper::getInstance()->parseParamsField(response, "groups");
}

void API::getMemberId() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password }
		}
		);

	CREATE_EVENT_LOG("Request to get member id");

	std::optional<const std::string> requestResult = performApiRequest(jsonString);
	if (!requestResult.has_value()) {
		CREATE_EVENT_LOG("Failed to get 'id' field value");
		return;
	}

	const std::string response = requestResult.value();
	if (response.empty()) {
		CREATE_EVENT_LOG("The response result is empty");
		return;
	}

	m_authData.member_id = JsonWrapper::getInstance()->parseParamsField(response, "id");
}


bool API::checkUserAuthentication() {
	getMemberId();

	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password }
		}
		);

	CREATE_EVENT_LOG("Request to verify account availability");

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserHwid() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "hwid" },
		  { "type", "update" }
		},
		{
			{ "member_id", m_authData.member_id },
			{ "hwid", m_authData.hwid }
		}
		);

	CREATE_EVENT_LOG("Request to verify hardware id");

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserLicense() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "license" },
		  { "type", "get" }
		},
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password }
		}
		);

	CREATE_EVENT_LOG("Request to verify the availability license");

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserToken() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "session" },
		  { "type", "validate" }
		},
		{
			{ "username", m_authData.username },
			{ "hwid", m_authData.hwid },
			{ "token", m_authData.token }
		}
		);

	CREATE_EVENT_LOG("Request to verify session token");

	return performApiRequest(jsonString).has_value();
}

std::optional<const std::string> API::performApiRequest(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::runtime_error("Function call error: empty argument (JSON string)");
	}

	const std::string encryptedJson = DataEncryption::encryptBase64(jsonString);
	if (encryptedJson.empty()) {
		CREATE_EVENT_LOG("Failed to initialize encrypted JSON");
		return std::nullopt;
	}

	boost::format source = boost::format("%1%?data=%2%") % m_url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	if (response.empty()) {
		CREATE_EVENT_LOG("Failed to receive a response from the Web API");
		return std::nullopt;
	}

	const std::string decryptedResponse = DataEncryption::decryptBase64(response);
	if (decryptedResponse.empty()) {
		CREATE_EVENT_LOG("Failed to decrypt the received response");
		return std::nullopt;
	}

	if (JsonWrapper::getInstance()->haveErrorField(decryptedResponse)) {
		CREATE_EVENT_LOG("The WEB API rejected the user's request");
		return std::nullopt;
	}

	CREATE_EVENT_LOG("The WEB API granted access to this request");

	return decryptedResponse;
}

const std::string API::getUsername() {
	if (m_authData.username.empty()) {
		CREATE_EVENT_LOG("The 'username' field is empty");
		return std::string();
	}
	return m_authData.username;
}
