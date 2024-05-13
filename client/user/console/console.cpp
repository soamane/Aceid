#include "console.h"

#include <iostream>

#include "../../client/api/api.h"
#include "../../user/hardware/hardware.h"

const std::string Console::getUserCredentials() {
	std::unique_ptr<AuthData> authData = std::make_unique<AuthData>();
	std::unique_ptr<API> api = std::make_unique<API>(authData.get());

	std::cout << "username: ";
	std::cin >> authData->username;

	std::cout << "password: ";
	std::cin >> authData->password;

	authData->hwid = Hardware::getHardwareId();
	if (authData->hwid.empty()) {
		throw std::runtime_error("Failed to get hardware id");
	}

	authData->token = api->getSessionToken();
	if (authData->token.empty()) {
		throw std::runtime_error("Failed to get session token");
	}

	const std::string convertedAuthData = api->convertAuthDataToJson();
	if (convertedAuthData.empty()) {
		throw std::runtime_error("Failed to get auth data");
	}

	return convertedAuthData;
}

void Console::showConsoleMessage(const std::string& message) {
	if (message.empty()) {
		throw std::invalid_argument("Function call error: empty argument (console message)");
	}

	std::cout << message << std::endl;
}

void Console::clearConsole() {
	system("cls");
}
