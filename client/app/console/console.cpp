#include "console.h"

#include <iostream>

#include "../../client/api/api.h"
#include "../../app/hardware/hardware.h"

#include "../../general/protect/xorstring/xorstring.h"

const std::string Console::getUserCredentials() {
	std::unique_ptr<AuthData> authData = std::make_unique<AuthData>();
	std::unique_ptr<API> api = std::make_unique<API>(authData.get());

	showConsoleMessage(xorstr_("username: "));
	std::cin >> authData->username;

	showConsoleMessage(xorstr_("password: "));
	std::cin >> authData->password;

	authData->hwid = Hardware::getHardwareId();
	if (authData->hwid.empty()) {
		throw std::runtime_error(xorstr_("Failed to get hardware id"));
	}

	authData->token = api->getSessionToken();
	if (authData->token.empty()) {
		throw std::runtime_error(xorstr_("Failed to get session token"));
	}

	const std::string convertedAuthData = api->convertAuthDataToJson();
	if (convertedAuthData.empty()) {
		throw std::runtime_error(xorstr_("Failed to get auth data"));
	}

	Console::clearConsole();
	return convertedAuthData;
}

void Console::showConsoleMessage(const std::string& message) {
	if (message.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (console message)"));
	}

	std::cout << message;
}

void Console::clearConsole() {
	system(xorstr_("cls"));
}
