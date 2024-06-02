#include "console.h"

#include <iostream>

#include "../../client/api/api.h"

#include "../../general/hardware/hardware.h"
#include "../../general/protect/xorstring/xorstring.h"

const std::string Console::GetUserCredentials(AuthData& authData, API& api) {
	PrintConsoleMessage(xorstr_("USERNAME: "));
	std::cin >> authData.username;

	PrintConsoleMessage(xorstr_("PASSWORD: "));
	std::cin >> authData.password;

	authData.hwid = Hardware::GetHardwareId();
	if (authData.hwid.empty()) {
		throw std::runtime_error(xorstr_("Failed to get hardware id"));
	}

	authData.launchParams = api.CreateLaunchParams();
	if (authData.launchParams.empty()) {
		throw std::runtime_error(xorstr_("Failed to get launch params"));
	}

	authData.token = api.GetSessionToken();
	if (authData.token.empty()) {
		throw std::runtime_error(xorstr_("Failed to get session token"));
	}

	const std::string credentials = api.ConvertAuthDataToJson();
	if (credentials.empty()) {
		throw std::runtime_error(xorstr_("Failed to get auth data"));
	}

	Console::Clear();
	return credentials;
}

void Console::PrintConsoleMessage(const std::string& message) {
	if (message.empty()) {
		throw std::invalid_argument(xorstr_("Function call error: empty argument (console message)"));
	}

	std::cout << message;
}

void Console::Clear() {
	system(xorstr_("cls"));
}
