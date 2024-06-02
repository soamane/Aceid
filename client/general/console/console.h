#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

#include "../../client/api/api.h"

class Console {
public:
	Console();
	~Console();

	const std::string GetUserCredentials(AuthData& authData, API& api);
	static void PrintConsoleMessage(const std::string& message);
	static void Clear();

private:
	void SetConsoleProperties();
	std::string GetHiddenInput();
private:
	const int m_consoleWidth = 50;
	const int m_consoleHeight = 15;
};

#endif // !CONSOLE_H
