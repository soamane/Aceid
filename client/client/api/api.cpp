#include "api.h"

#include "../../secure/crypt/crypt.h"

#include <stdexcept>
#include <boost/format.hpp>

API::API(AuthData* data) : data(data) {
}

const std::string API::convertAuthDataToJson() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" },
		  { "type", "login" }
		},
		{
			{ "username", data->username },
			{ "password", data->password },
			{ "hwid", data->hwid },
			{ "token", data->token }
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
			{ "username", data->username },
			{ "hwid", data->hwid }
		}
	);
	return performGetSessionToken(jsonString);
}

const std::string API::performGetSessionToken(const std::string& jsonString) {
	std::string encryptedJson = Crypt::encryptBase64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = Crypt::decryptBase64(response);

	if (!JsonWrapper::getInstance()->haveTokenField(decryptedResponse)) {
		throw std::runtime_error("failed to get session token");
	}

	return JsonWrapper::getInstance()->parseSessionToken(decryptedResponse);
}
