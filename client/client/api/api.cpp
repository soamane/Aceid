#include "api.h"

#include "../../general/protect/dataencryption/dataencryption.h"

#include <stdexcept>

API::API(AuthData* data) : m_authData(data) { }

const std::string API::ConvertAuthDataToJson() {
	const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString
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

const std::string API::GetSessionToken() {
	const std::string jsonString = JsonWrapper::GetInstance()->CreateJsonString
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

	return PerformRequestToGetSessionToken(jsonString);
}

const std::string API::PerformRequestToGetSessionToken(const std::string& jsonString) {
	if (jsonString.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument"));
	}

	const std::string encryptedJson = DataEncryption::EncryptBase64(jsonString);
	const std::string fullUrl = m_url + xorstr_("?data=") + encryptedJson;

	const std::string response = CurlWrapper::GetInstance()->PerformRequest(RequestType::HTTPS, fullUrl, nullptr);
	const std::string decryptedResponse = DataEncryption::DecryptBase64(response);

	if (!JsonWrapper::GetInstance()->haveTokenField(decryptedResponse)) {
		throw std::runtime_error(xorstr_("Failed to get session token"));
	}

	return JsonWrapper::GetInstance()->ParseSessionToken(decryptedResponse);
}
