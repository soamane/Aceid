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
	authData->token = api->getSessionToken();

	const std::string convertedAuthData = api->convertAuthDataToJson();
	return convertedAuthData;
}

void Console::showConsoleMessage(const std::string& message) {
	std::cout << message << std::endl;
}

void Console::clearConsole() {
	system("cls");
}
