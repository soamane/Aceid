#include "app.h"

#include <stdexcept>

Application::Application() {
	SetConsoleTitleA(applicationName.c_str());
}

Application::~Application() {
	if (applicationHandle != nullptr) {
		CloseHandle(applicationHandle);
	}
}

void Application::createApplicationMutex() {
	applicationHandle = CreateMutexA(NULL, TRUE, applicationName.c_str());
	if (applicationHandle == nullptr) {
		throw std::runtime_error(xorstr_("Failed to create application handle"));
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(applicationHandle);
		throw std::runtime_error(xorstr_("Application is already running"));
	}
}