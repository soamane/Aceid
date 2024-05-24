#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"
#include "../../general/logsystem/logmanager/logmanager.h"

API::API(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	getUserData(jsonString); // get main info of user ( username, password etc. )
	getMemberId(); // fills member id into AuthData for perform user hwid request after ( based on parse data by getUserData method )
	getProfileGroupId(); // fills group id into AuthData for definition access to the cheats ( based on parse data by getUserData method )
}

const AuthStatus API::getAuthStatus() {
	if (!checkUserAuthentication()) {
		CREATE_EVENT_LOG("Invalid credentials");
		return AUTH_ERROR_INVALID_CREDENTIALS;
	}

	if (!checkUserHwid()) {
		CREATE_EVENT_LOG("Invalid hwid");
		return AUTH_ERROR_INVALID_HWID;
	}

	if (!checkUserLicense() && m_authData.profile_group.empty()) {
		CREATE_EVENT_LOG("No license");
		return AUTH_ERROR_INVALID_LICENSE;
	}

	if (!checkUserToken()) {
		CREATE_EVENT_LOG("Invalid token");
		return AUTH_ERROR_INVALID_TOKEN;
	}

	return AUTH_SUCCESS;
}

void API::getUserData(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	m_authData = JsonWrapper::getInstance()->parseUserData(jsonString);
}

void API::getProfileGroupId() {

	CREATE_EVENT_LOG("Request to get profile group");

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

	CREATE_EVENT_LOG("Request to get member id");

	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password }
		}
		);

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

	CREATE_EVENT_LOG("Request to verify account availability");

	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" } },
		{
			{ "username", m_authData.username },
			{ "password", m_authData.password }
		}
		);

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserHwid() {

	CREATE_EVENT_LOG("Request to verify hardware id");

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

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserLicense() {

	CREATE_EVENT_LOG("Request to verify the availability license");

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

	return performApiRequest(jsonString).has_value();
}

bool API::checkUserToken() {

	CREATE_EVENT_LOG("Request to verify session token");

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

	return performApiRequest(jsonString).has_value();
}

std::optional<const std::string> API::performApiRequest(const std::string& jsonString) {
	CREATE_EVENT_LOG("Detailed information on request: ");

	if (jsonString.empty()) {
		throw std::invalid_argument("Function call error: empty argument [" + std::string(__func__) + "]");
	}

	const std::string encryptedJson = DataEncryption::encryptBase64(jsonString);
	if (encryptedJson.empty()) {
		CREATE_EVENT_LOG("Failed to initialize encrypted JSON");
		return std::nullopt;
	}

	const std::string fullUrl = m_url + "?data=" + encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::HTTPS, fullUrl, nullptr);
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
		CREATE_EVENT_LOG("Request have error field");
		return std::nullopt;
	}

	CREATE_EVENT_LOG("Request performed successfully");
	CREATE_EVENT_LOG("Host response: " + response + "\n");

	return decryptedResponse;
}

const AuthData& API::getAuthDataObject() {
	return m_authData;
}
