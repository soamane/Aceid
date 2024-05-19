#include "user.h"

#include <stdexcept>

#include "../general/protect/xorstring/xorstring.h"

void User::createApplicationMutex() {
	appHandle = CreateMutexA(NULL, TRUE, xorstr_("Client"));
	if (appHandle == nullptr) {
		throw std::runtime_error(xorstr_("Failed to create application handle"));
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(appHandle);
		throw std::runtime_error(xorstr_("Application is already running"));
	}
}

User::~User() { 
	if (appHandle != nullptr) {
		CloseHandle(appHandle);
	}
}
