#include "api.h"

API::API(const AuthData& data) : data(data) {
}

const std::string API::convertAuthDataToJson() {
	const std::string jsonString = JsonWrapper::getInstance()->createJsonString
	(
		{ { "action", "auth" },
		  { "type", "login" }
		},
		{
			{ "username", this->data.username },
			{ "password", this->data.password },
			{ "hwid", this->data.hwid },
			{ "token", this->data.token }
		}
	);

	return jsonString;
}
