#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"

#include <stdexcept>
#include <boost/format.hpp>

API::API(AuthData* data) : m_authData(data) { }

const std::string API::convertAuthDataToJson() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { xorstr_("action"), xorstr_("auth") },
		  { xorstr_("type"), xorstr_("login") }
		},
		{
			{ xorstr_("username"), m_authData->username },
			{ xorstr_("password"), m_authData->password },
			{ xorstr_("hwid"), m_authData->hwid },
			{ xorstr_("token"), m_authData->token }
		}
		);

	if (jsonString.empty()) {
		throw std::runtime_error(xorstr_("Failed to convert auth data to json"));
	}

	return jsonString;
}

const std::string API::getSessionToken() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { xorstr_("action"), xorstr_("session") },
		  { xorstr_("type"), xorstr_("create") }
		},
		{
			{ xorstr_("username"), m_authData->username },
			{ xorstr_("hwid"), m_authData->hwid }
		}
		);

	if (jsonString.empty()) {
		throw std::runtime_error(xorstr_("Failed to convert data for token"));
	}

	return performGetSessionToken(jsonString);
}

const std::string API::performGetSessionToken(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument"));
	}

	const std::string encryptedJson = DataEncryption::encryptBase64(jsonString);
	const boost::format source = boost::format(xorstr_("%1%?data=%2%")) % m_url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	const std::string decryptedResponse = DataEncryption::decryptBase64(response);

	if (!JsonWrapper::getInstance()->haveTokenField(decryptedResponse)) {
		throw std::runtime_error(xorstr_("Failed to get session token"));
	}

	return JsonWrapper::getInstance()->parseSessionToken(decryptedResponse);
}
