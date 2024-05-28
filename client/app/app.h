#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <string>

#include "../general/protect/xorstring/xorstring.h"

class Application {
public:
	Application();
	~Application();

	void CreateApplicationMutex();

private:
	HANDLE applicationHandle = nullptr;
	const std::string applicationName = xorstr_("ACEID");
};

#endif // !APPLICATION_H
