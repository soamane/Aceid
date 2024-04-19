#include "api.h"

#include "../../extern/base64/base64.h"
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
			{ "username", this->data->username },
			{ "password", this->data->password },
			{ "hwid", this->data->hwid },
			{ "token", this->data->token }
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
			{ "username", this->data->username },
			{ "hwid", this->data->hwid }
		}
	);
	return this->performGetSessionToken(jsonString);
}

const std::string API::performGetSessionToken(const std::string& jsonString) {
	std::string encryptedJson = base64::to_base64(jsonString);
	boost::format source = boost::format("%1%?data=%2%") % this->url % encryptedJson;

	const std::string response = CurlWrapper::getInstance()->performRequest(RequestType::eRT_HTTPS, source.str(), nullptr);
	std::string decryptedResponse = base64::from_base64(response);

	if (!JsonWrapper::getInstance()->haveTokenField(decryptedResponse)) {
		return std::string();
	}

	return JsonWrapper::getInstance()->parseSessionToken(decryptedResponse);
}
