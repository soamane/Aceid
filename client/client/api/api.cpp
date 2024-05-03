#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"

#include <stdexcept>
#include <boost/format.hpp>

API::API(AuthData* data) : m_authData(data) { }

const std::string API::convertAuthDataToJson() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" },
		  { "type", "login" }
		},
		{
			{ "username", m_authData->username },
			{ "password", m_authData->password },
			{ "hwid", m_authData->hwid },
			{ "token", m_authData->token }
		}
		);

	return jsonString;
}

const std::string API::getSessionToken() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "session" },
		  { "type", "create" }
		},
		{
			{ "username", m_authData->username },
			{ "hwid", m_authData->hwid }
		}
		);
	return performGetSessionToken(jsonString);
}

const std::string API::performGetSessionToken(const std::string& jsonString) {
	std::string encryptedJson = DataEncryption::encryptBase64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % m_url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = DataEncryption::decryptBase64(response);

	if (!JsonWrapper::getInstance()->haveTokenField(decryptedResponse)) {
		throw std::runtime_error("failed to get session token");
	}

	return JsonWrapper::getInstance()->parseSessionToken(decryptedResponse);
}
